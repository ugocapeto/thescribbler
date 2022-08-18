#include "header.h"

void point_set(
 arrayHeader *point_container,
 int point,
 double x,
 double y
)

{

 point_struct point_data;

 point_data.x= x;
 point_data.y= y;

 arrayPut(*point_container,point,point_data,point_struct);

}
