#include "header.h"

void point_get(
 arrayHeader *point_container,
 int point,
 double *px,
 double *py
)

{

 point_struct point_data;
 double x;
 double y;

 point_data= arrayGet(*point_container,point_struct,point);

 x= point_data.x;
 y= point_data.y;

 (*px)= x;
 (*py)= y;

}
