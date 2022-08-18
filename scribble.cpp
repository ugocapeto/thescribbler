int scribble(
	// input
	const Mat &src,
	const Mat &gray,
	const Mat &bg_mask,
	const Mat theEdgeMap,
	const vector<Point2i> &sequence,
	vector<Point2f> &scribble_points
){
	int jitter = 1;
	int mapping_mode = 0;
	bool finish = false;

	Mat output = Mat(gray.rows, gray.cols, CV_8UC1);
	output = WHITE;
	Mat intensityMap;
	Mat colorTrans = src;

	bool firstColorTrans = true;
	bool isColorTrans = false;

	///////////////////////////////////////////////////////////////////////////
	// values depend on image size
	float dtmin = DELTA_T_MIN;
	float dtmax = DELTA_T_MAX;

	// these two rmin/rmax should be the same as the ones in sampling function
	float rmin = SCRIBBLE_R_MIN;
	float rmax = SCRIBBLE_R_MAX;
	///////////////////////////////////////////////////////////////////////////

	// For Canny edge parameters
	bool adjust_radius_edge = true;
	bool adjust_radius_edge_MIN_switch = true;
	const float adjust_radius_edge_MIN = 0;
	Mat edge_image = theEdgeMap;

	const int n = sequence.size();
	intensityMap = imread(ORI_INPUT_PATH + ORI_INPUT_NAME, CV_LOAD_IMAGE_GRAYSCALE);

	Mat normal_map;
	Mat noise_map;
	if(config.CHANGE_NORMAL && config.READ_NORMAL_MAP){
		normal_map = imread(NORMAL_MAP_PATH + NORMAL_MAP_NAME, CV_LOAD_IMAGE_COLOR);
	}

	noise_map = Mat(gray.rows/2, gray.cols/2, CV_8UC1);
	randu(noise_map, 0, 255);
	GaussianBlur(noise_map, noise_map, Size(51,51), 0, 0, BORDER_DEFAULT);
	resize(noise_map, noise_map, Size(gray.cols, gray.rows));
	cv::normalize(noise_map, noise_map, 0, 255, NORM_MINMAX, CV_8UC1);
	resize(noise_map, noise_map, Size(gray.cols, gray.rows));

	if(!normal_map.data){
		normal_map = noise_map.clone();
	}

	writeMat("(4) normal_map.png", normal_map);

	scribble_points.clear();
	scribble_points.reserve(sequence.size()*3);
	printf("\n\n==== Start Circular Scribbling. ====\n");
	{ 
		clock_t t1 = clock();

		float theta = 0;

		Point2f vec2_previous(0, 0); // previous x, y
		Point2f vec2_next(0, 0); // next x, y

		float sample_ori_size_scale;

		if( theEdgeMap.cols > theEdgeMap.rows){
			sample_ori_size_scale = (float)intensityMap.cols / (float)config.MAX_WIDTH;
		}else{
			sample_ori_size_scale = (float)intensityMap.rows / (float)config.MAX_WIDTH;
		}

		vec2_previous = sequence[0];

		for(int i=1;i<n;i++){
			float tmp_dr, tmp_dt;
			float radius, dt;

			Point2f vec2_start(sequence[i-1]); //  start point of a single segment of the path
			Point2f vec2_target(sequence[i]);  // target point of a single segment of the path

			float t;  // tracing process of a single segment of the path
			t = 0;

			float dist;
			float dx, dy;
			dx = vec2_target.x - vec2_start.x;
			dy = vec2_target.y - vec2_start.y;
			dist = sqrt(dx*dx+dy*dy);

			while(t<1.0){
				bool isForeGround = true;
				Point2f vec2_center(vec2_start);
				vec2_center = vec2_start + t * Point2f(dx, dy);

				radius = SCRIBBLE_R_MIN;
				dt = 1;
				Point2i sample_ori_point;
				sample_ori_point = vec2_center * sample_ori_size_scale;
				if(isPointInMat(sample_ori_point, intensityMap)){
					// sample ori size image
					float gray_scale;
					gray_scale = intensityMap.at<uchar>(sample_ori_point)/255.0f; // dark 0~1 bright
				
					tmp_dr = mapping_func(gray_scale, rmin, rmax, jitter);
					tmp_dt = mapping_func(gray_scale, dtmin, dtmax, 0);

					// makes bg scribble different from fg
					if(bg_mask.data){
						if(bg_mask.at<uchar>(vec2_center) < 128){
							tmp_dr = rmax * (1.3 + 0.1 * sinf(rand()));
							tmp_dt = dtmax* (1.3 + 0.1 * sinf(rand()));
							isForeGround = false;
						}
					}

					// adjust radius along Canny edge.
					if(config.REDUCE_RADIUS){
						if(adjust_radius_edge == true){
							tmp_dr = getReducedRadius(
								vec2_center, gray, edge_image, tmp_dr, 
								adjust_radius_edge_MIN_switch, 
								adjust_radius_edge_MIN);
						}
					}
					radius = tmp_dr;
					dt = tmp_dt;
				}
				t += dt / dist;

				float X, Y;

				// ellipse formula
				float __a = radius;
				float __b = radius;

				// normal orientation theta_z
				float theta_z = 0.0f;

				if(config.CHANGE_NORMAL){
					if(isForeGround){
						if(config.READ_NORMAL_MAP){
							// 0 ~ 255
							float r = (float)normal_map.at<Vec3b>(Point(vec2_center.x, vec2_center.y))[2];
							float g = (float)normal_map.at<Vec3b>(Point(vec2_center.x, vec2_center.y))[1];
							float b = (float)normal_map.at<Vec3b>(Point(vec2_center.x, vec2_center.y))[0];

							// -180 ~ +180
							float x = r / 255.0f * 2.0f - 1.0f;
							float y = g / 255.0f * 2.0f - 1.0f;
							float z = b / 255.0f * 2.0f - 1.0f;

							// apply 70% of normal map value, not tilt too much
							float alpha = 0.9f * acosf(z/sqrtf(x*x+y*y+z*z));
							float phi = atanf(y/x) + M_PI / 2.0f;

							alpha += ((float)(rand()%(1+(int)(alpha*config.ALPHA_NOISE)))) / 180.0f * M_PI;
							phi   += ((float)(rand()%(1+config.PHI_NOISE))) / 180.0f * M_PI;

							__b = radius * cosf(alpha);
							theta_z = phi;
						}else{
							__b = radius * cosf(40.0f / 180.0f * M_PI);
							theta_z = ((float)(noise_map.at<uchar>(Point(vec2_center.x, vec2_center.y)))/255.0f*45.0f+45.0f) / 180.0f * M_PI;
						}
					}
				}

				// local point position
				X = __a * cosf((theta) / 180.0f * M_PI);
				Y = __b * sinf((theta) / 180.0f * M_PI);

				// rotate by z-axis
				float tmp_X =  cosf(theta_z) * X + sinf(theta_z) * Y;
				float tmp_Y = -sinf(theta_z) * X + cosf(theta_z) * Y;
				X = tmp_X;
				Y = tmp_Y;
				
				// translate offset to the path
				X += vec2_center.x;
				Y += vec2_center.y;

				vec2_next = Point2f(X, Y);

				scribble_points.push_back(vec2_next);
				vec2_previous = vec2_next;
				theta += 20;
			}
		}
		time_elapsed_secs = (double)(clock() - t1) / CLOCKS_PER_SEC; 
	}
	mylog.time_scribble = time_elapsed_secs;
	printf("Scribble time : %.3lf secs\n", mylog.time_scribble);

	SEQUENCE_STRING = OUTPUT_PATH + ORI_INPUT_NAME + "_sequence_" + TIME_STRING + ".txt";
	FILE *fout = fopen(SEQUENCE_STRING, "w");
	const int steps = 5;
	fprintf(fout, "%d %d\n%d\n", output.rows, output.cols, scribble_points.size()/steps);
	for(int i=0;i<scribble_points.size();i+=steps){
		fprintf(fout, "%f %f\n", scribble_points[i].x, scribble_points[i].y);
	}
	fclose(fout);

	return 0;
}
