#include "header.h"

void scribble_b(
 arrayHeader *tracing_path,
 int inp_point_ind,
 double inp_point_dist,
 double cur_dist,
 int *pout_point_ind,
 double *pout_point_dist,
 double *pvec2_center_x,
 double *pvec2_center_y
)

{

 int out_point_ind;
 double out_point_dist;
 double vec2_center_x;
 double vec2_center_y;
 int maxId;
 int point_nbr;
 int point_ind;
 double dist;
 double vec2_start_x;
 double vec2_start_y;
 double vec2_target_x;
 double vec2_target_y;
 double dx;
 double dy;
 double segment_dist;
 double next_dist;
 double lambda;

 out_point_ind=-1;
 out_point_dist= 0.0;
 vec2_center_x= 0.0;
 vec2_center_y= 0.0;

 maxId= arrayMaxId(*tracing_path);
 point_nbr= maxId+1;

 dist= inp_point_dist;

 for ( point_ind= inp_point_ind ; point_ind< point_nbr ; point_ind++ ) {

    point_get(
     tracing_path,
     point_ind,
     &vec2_start_x,
     &vec2_start_y
    );

    point_get(
     tracing_path,
     (point_ind+1)%point_nbr,
     &vec2_target_x,
     &vec2_target_y
    );

    dx = vec2_target_x - vec2_start_x;
    dy = vec2_target_y - vec2_start_y;
    segment_dist = sqrt(dx*dx+dy*dy);

    next_dist= dist+segment_dist;

    if ( cur_dist >= dist && cur_dist < next_dist ) {
       lambda= (cur_dist-dist)/segment_dist;
       vec2_center_x= vec2_start_x+lambda*dx;
       vec2_center_y= vec2_start_y+lambda*dy;
       out_point_ind= point_ind;
       out_point_dist= dist;
       break;
    }

    dist= next_dist; 
 }

 (*pout_point_ind)= out_point_ind;
 (*pout_point_dist)= out_point_dist;
 (*pvec2_center_x)= vec2_center_x;
 (*pvec2_center_y)= vec2_center_y;

}
