double hermite_curve_recovering(Mat &image){
	FILE *in = fopen(SEQUENCE_STRING, "r");
	int rows, cols;
	fscanf(in, "%d %d", &rows, &cols);
	int n;
	fscanf(in, "%d", &n);

	vector<Point2f> seq;
	seq.reserve(n);

	for(int i=0;i<n;i++){
		float x, y;
		fscanf(in, "%f %f", &x, &y);
		//printf("%f %f\n", x, y);
		//if(i%5!=0) continue;
		seq.push_back(Point2f(x, y));
	}
	fclose(in);

	Point2f P1, P2, T1, T2, pre;
	float _max = 0;
	float _min = 10000000000;
	vector<Point2f> seq_hermite;
	seq_hermite.clear();
	seq_hermite.reserve(n*20);

	P1 = seq[0];
	pre = P1;
	int size = seq.size();
	for(int i=1;i<size;i++){
		if(i==1){
			P1 = seq[0];
			P2 = seq[1];
			T1 = P2-P1;
			T2 = seq[2]-P1;
		}else if(i==seq.size()-1){
			P1 = seq[i-1];
			P2 = seq[i];
			T1 = P2-seq[i-2];
			T2 = P2-P1;
		}else{
			P1 = seq[i-1];
			P2 = seq[i];
			//T1 = P2-seq[i-2];
			T1 = T2;
			T2 = seq[i+1]-P1;
		}
				
		float d = dist(P1, P2);
		_max = my_max(_max, d);
		_min = my_min(_min, d);

		int steps = 20;
		//printf("%d\n", tmp_step);
		for(int t=0;t<steps;t++){
			float s = (float)t / (float)steps;    // scale s to go from 0 to 1
			float s2 = s*s;
			float s3 = s2*s;
			float h1 =  2*s3 - 3*s2 + 1;          // calculate basis function 1
			float h2 = -2*s3 + 3*s2;              // calculate basis function 2
			float h3 =    s3 - 2*s2 + s;          // calculate basis function 3
			float h4 =    s3 -   s2;              // calculate basis function 4
			Point2f p = h1*P1 +                     // multiply and sum all funtions
						h2*P2 +                     // together to build the interpolated
						h3*T1 +                     // point along the curve.
						h4*T2;
			//line(image, pre, p, 0);     // draw to calculated point on the curve
			//printf("%.5f %.5f -> %.5f %.5f\n", pre.x, pre.y, p.x, p.y);
			if(t==0) seq_hermite.push_back(pre);
			seq_hermite.push_back(p);
			pre = p;
		}
	}

	int lineWidth = config.LINE_WIDTH;
	float _scale = 1.667837f;

	image = cv::Mat((int)(rows*_scale), (int)(cols*_scale), CV_8UC1);
	image = cv::Scalar::all(255);
	//image.convertTo(image, CV_32FC1);
	//image = 1.0f;
	for(int i=1;i<seq_hermite.size();i++){
		cv::Point2f p1 = _scale*cv::Point2f(seq_hermite[i-1].x, seq_hermite[i-1].y);
		cv::Point2f p2 = _scale*cv::Point2f(seq_hermite[i].x, seq_hermite[i].y);
		//line(image, p1, p2, 0, (int)lineWidth, CV_AA);
		line(image, p1, p2, 0, (int)lineWidth);
	}
	//image.convertTo(image, CV_8UC1);
	//image *= 255.0f;

	string output_result_path = OUTPUT_PATH+"output_"+itoa(lineWidth, "%d")+"px_"+ftoa(_scale, "%f")+"_"+getTimeString()+".png";
	cv::imwrite(output_result_path, image);
	system(("start " + output_result_path).c_str());
	return 0;
}
