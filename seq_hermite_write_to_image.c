#include "header.h"

void seq_hermite_write_to_image(
 int width,
 int height,
 int *image_arr,
 arrayHeader *seq_hermite
)

{

 int i;
 int j;
 int pixel;
 int maxId;
 int point_nbr;
 int point;
 double x;
 double y;

 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       pixel= i*width+j;
       image_arr[pixel]= 255;
    }
 }

 maxId= arrayMaxId(*seq_hermite);
 point_nbr= maxId+1;

 for ( point= 0 ; point< point_nbr ; point++ ) {

    point_get(
     seq_hermite,
     point,
     &x,
     &y
    );

    j= (int)round(x);

    if ( j < 0 )
     continue;
    if ( j > width-1 )
     continue; 
 
    i= (int)round(y);

    if ( i < 0 )
     continue;
    if ( i > height-1 )
     continue;

    pixel= i*width+j;
    image_arr[pixel]= 0;
 }

}
