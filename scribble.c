#include "header.h"

void scribble(
 image_struct intensityMap,
 arrayHeader *tracing_path,
 arrayHeader *scribble_points,
 double rmin,
 double rmax,
 double dtmin,
 double dtmax,
 double dtheta
)

{

 double theta;
 double tmp_dr;
 double tmp_dt;
 double radius;
 double dt;
 double vec2_center_x;
 double vec2_center_y;
 double gray_scale;
 double X;
 double Y;
 double __a;
 double __b;
 double theta_z;
 double pi = acos(-1.0);
 int maxId;
 double intensity_dbl;
 double tmp_X;
 double tmp_Y;
 int point;
 int vec2_center_x_int;
 int vec2_center_y_int;
 int pixel;
 int intensity;
 int point_nbr;
 int cur_point_ind;
 double cur_point_dist;
 double cur_dist;
 int next_point_ind;
 double next_point_dist;

 maxId= arrayMaxId(*tracing_path);
 point_nbr= maxId+1;

 theta= 0.0;

 cur_point_ind= 0;
 cur_point_dist= 0.0;
 cur_dist= 0.0;

 START:

 /*
 Get scribble center
 */

 scribble_b(
  tracing_path,
  cur_point_ind,
  cur_point_dist,
  cur_dist,
  &next_point_ind,
  &next_point_dist,
  &vec2_center_x,
  &vec2_center_y
 );

 /*
 Check for termination
 If the current distance is greater than the tour distance,
 we are done!
 */

 if ( next_point_ind == -1 )
  goto END;

 /*
 gray_scale is the gray scale value at the center position
 dark 0~1 bright
 */

 intensity_dbl= bicubic_interpolation_on_image_int(
  intensityMap.image_arr,
  vec2_center_x,
  vec2_center_y,
  intensityMap.width,
  intensityMap.height
 );

 if ( intensity_dbl < 0.0 )
  intensity_dbl= 0.0; 
 if ( intensity_dbl > 255.0 )
  intensity_dbl= 255.0; 

 gray_scale= intensity_dbl/255.0;

 /*
 Let's get the gray scale value in a different way!
 */

 vec2_center_x_int= (int)round(vec2_center_x);

 if ( vec2_center_x_int < 0 )
  vec2_center_x_int= 0;
 if ( vec2_center_x_int > intensityMap.width-1 )
  vec2_center_x_int= intensityMap.width-1;

 vec2_center_y_int= (int)round(vec2_center_y);

 if ( vec2_center_y_int < 0 )
  vec2_center_y_int= 0;
 if ( vec2_center_y_int > intensityMap.height-1 )
  vec2_center_y_int= intensityMap.height-1;

 pixel= vec2_center_y_int*intensityMap.width+vec2_center_x_int;
 intensity= intensityMap.image_arr[pixel];

 gray_scale= (double)intensity/255.0;

 tmp_dr= mapping_func(gray_scale, rmin, rmax);
 tmp_dt= mapping_func(gray_scale, dtmin, dtmax);

 radius = tmp_dr;
 dt= tmp_dt;

 /*
 ellipse formula
 */

 __a = radius;
 __b = radius;

 /*
 normal orientation theta_z
 */

 theta_z = 0.0;

 /*
 local point position
 */

 X = __a * cos(theta / 180.0 * pi);
 Y = __b * sin(theta / 180.0 * pi);

 /*
 rotate by z-axis
 */ 

 tmp_X =  cos(theta_z / 180.0 * pi) * X + sin(theta_z / 180.0 * pi) * Y;
 tmp_Y = -sin(theta_z / 180.0 * pi) * X + cos(theta_z / 180.0 * pi) * Y;
 X = tmp_X;
 Y = tmp_Y;

 /*
 translate offset to the path
 */ 

 X += vec2_center_x;
 Y += vec2_center_y;

 /*
 Add point to scribble points
 */

 point= point_add(
  scribble_points,
  X,
  Y 
 );

 /*
 Increment the scribble rotation
 */

 theta += dtheta;

 /*
 Update the distances along the tracing path
 */

 cur_point_ind= next_point_ind;
 cur_point_dist= next_point_dist;
 cur_dist+= dt;

 goto START;

 END: ;

}
