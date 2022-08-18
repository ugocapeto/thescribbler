#include "header.h"

void tracing_path_write_to_image(
 int width,
 int height,
 int *image_arr,
 arrayHeader *tracing_path
)

{

 int i;
 int j;
 int pixel;
 int maxId;
 int point_nbr;
 int point_ind;
 double p0_x_dbl;
 double p0_y_dbl;
 int p0_x_int;
 int p0_y_int;
 double p1_x_dbl;
 double p1_y_dbl;
 int p1_x_int;
 int p1_y_int;
 int *xy_int_arr;
 int xy_int_nbr;
 int xy_int_ind;
 int p_x_int;
 int p_y_int;

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       image_arr[pixel]= 255;
    }
 }

 maxId= arrayMaxId(*tracing_path);
 point_nbr= maxId+1;

 for ( point_ind= 0 ; point_ind< point_nbr ; point_ind++ ) {

    point_get(
     tracing_path,
     point_ind,
     &p0_x_dbl,
     &p0_y_dbl
    );

    p0_x_int= (int)round(p0_x_dbl);
    if ( p0_x_int < 0 )
     p0_x_int= 0;
    if ( p0_x_int > width-1 )
     p0_x_int= width-1;

    p0_y_int= (int)round(p0_y_dbl);
    if ( p0_y_int < 0 )
     p0_y_int= 0;
    if ( p0_y_int > height-1 )
     p0_y_int= height-1;

    point_get(
     tracing_path,
     (point_ind+1)%point_nbr,
     &p1_x_dbl,
     &p1_y_dbl
    );

    p1_x_int= (int)round(p1_x_dbl);
    if ( p1_x_int < 0 )
     p1_x_int= 0;
    if ( p1_x_int > width-1 )
     p1_x_int= width-1;

    p1_y_int= (int)round(p1_y_dbl);
    if ( p1_y_int < 0 )
     p1_y_int= 0;
    if ( p1_y_int > height-1 )
     p1_y_int= height-1;

    plot_line(
     p0_x_int,
     p0_y_int,
     p1_x_int,
     p1_y_int,
     &xy_int_arr,
     &xy_int_nbr
    );

    for ( xy_int_ind= 0 ; xy_int_ind< xy_int_nbr ; xy_int_ind++ ) {
       p_x_int= xy_int_arr[2*xy_int_ind+0];
       p_y_int= xy_int_arr[2*xy_int_ind+1];
       i= p_y_int;
       j= p_x_int;
       pixel= i*width+j;
       image_arr[pixel]= 0;
    }

    if ( xy_int_nbr >  0 )
     myFree(xy_int_arr);
 }

}
