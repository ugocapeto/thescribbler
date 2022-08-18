#include "header.h"

double mapping_func(
 double x,
 double min,
 double max
)

{

 double result;

 result = mapping_func_kernel(x, min, max);

 return result;

}
