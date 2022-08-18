#include "header.h"

double get_reduced_radius(
 double p_x,
 double p_y,
 image_struct edge_image,
 int edge_image_threshold,
 double rori,
 double rmin
)

{

 double x;
 double y;
 double p[2];
 double dist_p_E;
 double min_dist_p_E;
 double p2[2];
 double vec[2];
 double intensity_dbl;
 double gray_scale;
 double r;
 int x_int;
 int y_int;
 int pixel;
 int intensity;

 p[0]= p_x;
 p[1]= p_y;

 min_dist_p_E=+1.0e32;
 for ( x=  p_x-rori ;
       x<= p_x+rori ;
       x+= 1.0 ) {

    if ( !(x >= 0) )
     continue;
    if ( !(x <= edge_image.width-1) )
     continue;

    for ( y=  p_y-rori ;
          y<= p_y+rori ;
          y+= 1.0 ) {

       if ( !(y >= 0) )
        continue;
       if ( !(y <= edge_image.height-1) )
        continue;

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

       /*
       Let's get the gray scale value in a different way!
       */

       x_int= (int)round(x);

       if ( x_int < 0 )
        x_int= 0;
       if ( x_int > edge_image.width-1 )
        x_int= edge_image.width-1;

       y_int= (int)round(y);

       if ( y_int < 0 )
        y_int= 0;
       if ( y_int > edge_image.height-1 )
        y_int= edge_image.height-1;

       pixel= y_int*edge_image.width+x_int;
       intensity= edge_image.image_arr[pixel];

       gray_scale= (double)intensity/255.0;

       /*
       if ( intensity_dbl <= (double)edge_image_threshold ) {
       */
       if ( intensity <= edge_image_threshold ) {

          /*
          Looks like we have reached an image feature edge
          */

          p2[0]= x;
          p2[1]= y;

          vector_diff_2d(p2,p,vec);
          dist_p_E= vector_norm_2d(vec);

          if ( dist_p_E < min_dist_p_E ) {
             min_dist_p_E= dist_p_E;
          }
       } 
    }
 }
 dist_p_E= min_dist_p_E;

 if ( dist_p_E < rmin ) {
    r= rmin;
 }
 if ( dist_p_E >= rmin && dist_p_E <= rori ) {
    r= dist_p_E;
 }
 if ( dist_p_E > rori ) {
    r= rori;
 }

 return r;

}
