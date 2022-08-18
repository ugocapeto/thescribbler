float getReducedRadius(
	Point2f vec2_center,
	const Mat& gray, 
	Mat& edge_image, 
	float tmp_dr, 
	bool adjust_radius_edge_MIN_switch, 
	float adjust_radius_edge_MIN
){
	float adjust_radius_edge_min;
	float result;

	float cx = vec2_center.x;
	float cy = vec2_center.y;

	result = tmp_dr;
	adjust_radius_edge_min = tmp_dr;
	for(float i=-tmp_dr ; i<=tmp_dr ; i+=1.0){
		for(float j=-tmp_dr ; j<=tmp_dr ; j+=1.0){
			int x = (int)(cx+j);
			int y = (int)(cy+i);
			if( x>1 && y>1 && x<gray.cols && y<gray.rows){
				//printf("IN PIC\n");
				if(j*j+i*i >= adjust_radius_edge_min){	
					//printf("IN SKIP\n");
					continue;
				}
				else if(edge_image.at<uchar>(y, x)>0){	
					//printf("ADJUST IT\n");
					adjust_radius_edge_min = sqrtf(j*j + i*i);
				}
			}
		}
	}
	if(adjust_radius_edge_MIN_switch && adjust_radius_edge_min < adjust_radius_edge_MIN){
		adjust_radius_edge_min = adjust_radius_edge_MIN;
	}
	result = adjust_radius_edge_min;
	return result;
}
