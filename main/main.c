#include "header.h"

int main(
 int argc,
 char* argv[]
)

{

 FILE *fp;
 char filename[MAXLINE];
 char filename_source_image[MAXLINE];
 char filename_tracing_path_points[MAXLINE];
 char filename_tracing_path_segments[MAXLINE];
 char filename_output_image[MAXLINE];
 arrayHeader *tracing_path;
 arrayHeader *scribble_points;
 image_struct intensityMap;
 double rmin;
 double rmax;
 double dtmin;
 double dtmax;
 int err_flag;
 int *source_image_arr;
 int width;
 int height;
 arrayHeader *seq_hermite;
 int *output_image_arr;
 int *tracing_path_image_arr;
 double dtheta;

 /*
 Let's read in the input file
 */

 fp= fopen("thescribbler_input.txt","r");
 if ( fp == NULL ) {
    fprintf(stdout,"thescribbler_input.txt not found!\n");
    return 1;
 }

 /*
 Get filename for source image
 */

 fscanf(fp,"%s",filename);

 fprintf(stdout,"source image = %s\n",filename);

 strcpy(filename_source_image,filename);

 /*
 Get filename for tracing path points
 */

 fscanf(fp,"%s",filename);

 fprintf(stdout,"tracing path points = %s\n",filename);

 strcpy(filename_tracing_path_points,filename);

 /*
 Get filename for tracing path segments
 */

 fscanf(fp,"%s",filename);

 fprintf(stdout,"tracing path segments = %s\n",filename);

 strcpy(filename_tracing_path_segments,filename);

 /*
 Get minimum disk radius
 */

 fscanf(fp,"%lg",&rmin);

 fprintf(stdout,"minimum disk radius = %g\n",rmin);

 /*
 Get maximum disk radius
 */

 fscanf(fp,"%lg",&rmax);

 fprintf(stdout,"maximum disk radius = %g\n",rmax);

 /*
 Get minimum center velocity
 */

 fscanf(fp,"%lg",&dtmin);

 fprintf(stdout,"minimum center velocity = %g\n",dtmin);

 /*
 Get maximum center velocity
 */

 fscanf(fp,"%lg",&dtmax);

 fprintf(stdout,"maximum center velocity = %g\n",dtmax);

 /*
 Get scribble rotation increment
 */

 fscanf(fp,"%lg",&dtheta);

 fprintf(stdout,"scribble rotation increment = %g\n",dtheta);

 /*
 Get filename for output image
 */

 fscanf(fp,"%s",filename);

 fprintf(stdout,"output image = %s\n",filename);

 strcpy(filename_output_image,filename);

 /*
 Done reading the input file
 */

 fclose(fp);

 /*
 Load source image
 */

 err_flag= load_image(
  filename_source_image,
  &source_image_arr,
  &width,
  &height
 );

 if ( err_flag == 1 ) {
    fprintf(stdout,"source image file %s not loaded!\n",filename_source_image);
    return 1;
 }

 intensityMap.width= width;
 intensityMap.height= height;
 intensityMap.image_arr= source_image_arr;

 /*
 Create tracing path container
 */

 point_container_new(
  &tracing_path
 );

 /*
 Load tracing path
 */

 tracing_path_load(
  filename_tracing_path_points,
  filename_tracing_path_segments,
  tracing_path
 );

 /*
 Allocate memory for the tracing path image
 */

 myCalloc(tracing_path_image_arr,int,width*height,sizeof(int));

 /*
 Write tracing path to tracing path image
 */

 tracing_path_write_to_image(
  width,
  height,
  tracing_path_image_arr,
  tracing_path
 );

 /*
 Write tracing path image
 */

 err_flag= write_image(
  (char *)"tracing_path_image.png",
  tracing_path_image_arr,
  width,
  height
 );

 if ( err_flag == 1 ) {
    fprintf(stdout,"tracing path image file %s not written!\n",(char *)"tracing_path_image.png");
    return 1;
 }

 /*
 Create scribble points container
 */

 point_container_new(
  &scribble_points
 );

 /*
 Fill scribble points container
 */

 scribble(
  intensityMap,
  tracing_path,
  scribble_points,
  rmin,
  rmax,
  dtmin,
  dtmax,
  dtheta
 );

 /*
 Create hermite curve points container
 */

 point_container_new(
  &seq_hermite
 );

 /*
 Fill hermite curve points container
 */

 hermite_curve_recovering(
  scribble_points,
  seq_hermite
 );

 /*
 Allocate memory for the output image
 */

 myCalloc(output_image_arr,int,width*height,sizeof(int));

 /*
 Fill output image
 */

 seq_hermite_write_to_image(
  width,
  height,
  output_image_arr,
  seq_hermite
 );

 /*
 Write output image
 */

 err_flag= write_image(
  filename_output_image,
  output_image_arr,
  width,
  height
 );

 if ( err_flag == 1 ) {
    fprintf(stdout,"output image file %s not written!\n",filename_output_image);
    return 1;
 }

 return 0;

}
