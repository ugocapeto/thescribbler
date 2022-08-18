#include "header.h"

double getReducedRadius(
 double vec2_center_x,
 double vec2_center_y,
 image_struct edge_image,
 double tmp_dr,
 bool adjust_radius_edge_MIN_switch,
 double adjust_radius_edge_MIN
)

{

 double adjust_radius_edge_min;
 double result;
 double cx;
 double cy;
 double i;
 double j;
 int x;
 int y;
 double intensity_dbl;
 double gray_scale;

 cx = vec2_center_x;
 cy = vec2_center_y;

 result = tmp_dr;
 adjust_radius_edge_min = tmp_dr;

 for ( i= -tmp_dr ; i<= tmp_dr ; i+=1.0 ) {
    for ( j= -tmp_dr ; j<= tmp_dr ; j+=1.0 ) {
       x = (int)(cx+j);
       y = (int)(cy+i);
       if ( x > 1 &&
            y > 1 &&
            x < edge_image.width &&
            y < edge_image.height ) {
          if ( j*j+i*i >= adjust_radius_edge_min ) {
             continue;
          }
          else {
             intensity_dbl= bicubic_interpolation_on_image_int(
              edge_image.image_arr,
              x,
              y,
              edge_image.width,
              edge_image.height
             );

             if ( intensity_dbl < 0.0 )
              intensity_dbl= 0.0;
             if ( intensity_dbl > 255.0 )
              intensity_dbl= 255.0;

             gray_scale= intensity_dbl/255.0;
             if ( intensity_dbl > 0 ) {
                adjust_radius_edge_min = sqrt(j*j + i*i);
             }
          }
       }
    }
 }

 if ( adjust_radius_edge_MIN_switch &&
      adjust_radius_edge_min < adjust_radius_edge_MIN ) {
    adjust_radius_edge_min = adjust_radius_edge_MIN;
 }

 result = adjust_radius_edge_min;

 return result;

}
