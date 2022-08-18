typedef struct point_struct {
 int tag; /* always used for garbage collection */
 double x;
 double y;
} point_struct;

typedef struct image_struct {
 int width;
 int height;
 int *image_arr; /* 0-255 */
} image_struct;
