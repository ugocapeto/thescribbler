#include "header.h"

void point_container_new(
 arrayHeader **ppoint_container
)

{

 arrayHeader *point_container;
 /* that's way too small!
 int block= 100;
 */
 int block= 100000;

 myMalloc(point_container,arrayHeader,sizeof(arrayHeader));
 arrayInit(*point_container,block);

 (*ppoint_container)= point_container;

}
