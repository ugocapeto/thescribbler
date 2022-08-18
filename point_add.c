#include "header.h"

int point_add(
 arrayHeader *point_container,
 double x,
 double y
)

{

 point_struct point_data;
 int tag;
 int index;

 tag= 0;

 if ( tag < 0 ) {
    error_handler((char *)"point_add");
 }

 point_data.tag= tag;
 point_data.x= x;
 point_data.y= y;

 arrayPutNext(*point_container,point_data,point_struct,tag,index);

 return index;

}
