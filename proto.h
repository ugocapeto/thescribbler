double get_reduced_radius(
 double p_x,
 double p_y,
 image_struct edge_image,
 int edge_image_threshold,
 double rori,
 double rmin
);

double getReducedRadius(
 double vec2_center_x,
 double vec2_center_y,
 image_struct edge_image,
 double tmp_dr,
 bool adjust_radius_edge_MIN_switch,
 double adjust_radius_edge_MIN
);

void hermite_curve_recovering(
 arrayHeader *scribble_points,
 arrayHeader *seq_hermite
);

double mapping_func(
 double x,
 double min,
 double max
);

double mapping_func_kernel(
 double x,
 double min,
 double max
);

int point_add(
 arrayHeader *point_container,
 double x,
 double y
);

void point_container_free(
 arrayHeader *point_container
);

void point_container_new(
 arrayHeader **ppoint_container
);

void point_get(
 arrayHeader *point_container,
 int point,
 double *px,
 double *py
);

void point_set(
 arrayHeader *point_container,
 int point,
 double x,
 double y
);

void scribble_b(
 arrayHeader *tracing_path,
 int inp_point_ind,
 double inp_point_dist,
 double cur_dist,
 int *pout_point_ind,
 double *pout_point_dist,
 double *pvec2_center_x,
 double *pvec2_center_y
);

void scribble(
 image_struct intensityMap,
 arrayHeader *tracing_path,
 arrayHeader *scribble_points,
 double rmin,
 double rmax,
 double dtmin,
 double dtmax,
 double dtheta
);

void seq_hermite_write_to_image(
 int width,
 int height,
 int *image_arr,
 arrayHeader *seq_hermite
);

void tracing_path_adjust(
 arrayHeader *tracing_path
);

void tracing_path_load(
 char *filename_tracing_path_points,
 char *filename_tracing_path_segments,
 arrayHeader *tracing_path
);

void tracing_path_write_to_image(
 int width,
 int height,
 int *image_arr,
 arrayHeader *tracing_path
);
