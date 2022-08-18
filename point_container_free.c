#include "header.h"

void point_container_free(
 arrayHeader *point_container
)

{

 arrayDelAll(*point_container);
 myFree(point_container);

}
