inline float mapping_func(float x, float min, float max, int jitter){
	float result = mapping_func_kernel(x, min, max);
	if(jitter){
		static float t = 0;
		result = result * (1.0f+(1-x)*0.5f*sinf(t));
		t+=4;
	}
	return result;
	return 0;
}
