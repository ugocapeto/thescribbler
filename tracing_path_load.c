#include "header.h"

void tracing_path_load(
 char *filename_tracing_path_points,
 char *filename_tracing_path_segments,
 arrayHeader *tracing_path
)

{

 FILE *fp;
 char *char_ptr;
 char *char_ptr2;
 char line[MAXLINE];
 double x;
 double y;
 int point;
 unsigned int char_ind;
 int point_nbr;
 double *point_arr;
 int point_ind;
 int index;
 int segment_nbr;
 int segment_nbr2;
 int segment_ind;
 int point_ind0;
 int point_ind1;
 int dist_int;
 int first_point_ind0;
 int prev_point_ind1;

 fp= fopen(filename_tracing_path_points,"r");
 if ( fp == NULL ) {
    fprintf(stderr,"Tracing path points file %s doesn't exist!\n",filename_tracing_path_points);
    error_handler((char *)"tracing_path_load");
 }

 /*
 Look for line that contains "DIMENSION"
 */

 while ((char_ptr= fgets(line,MAXLINE,fp)) != NULL ) {
    char_ptr2= strstr(line,"DIMENSION");
    if ( char_ptr2 != NULL )
     break;
 }

 if ( char_ptr == NULL ) {

    /*
    Looks like there is no line that contains "DIMENSION"
    */

    fprintf(stderr,"Tracing path file %s doesn't exist!\n",filename_tracing_path_points);
    error_handler((char *)"tracing_path_load");
 }

 /*
 Let's get the number of points
 Should be after ":" in that line
 */

 for ( char_ind= 0 ; char_ind< strlen(line) ; char_ind++ ) {
    if ( line[char_ind] == ':' )
     break;
 }
 char_ind++;

 sscanf(&line[char_ind],"%d",
  &point_nbr
 );

 /*
 We are gonna store the point coords in point_arr
 */

 myCalloc(point_arr,double,point_nbr,2*sizeof(double));

 /*
 Look for line that contains "NODE_COORD_SECTION"
 */

 while ((char_ptr= fgets(line,MAXLINE,fp)) != NULL ) {
    char_ptr2= strstr(line,"NODE_COORD_SECTION");
    if ( char_ptr2 != NULL )
     break;
 }

 if ( char_ptr == NULL ) {

    /*
    Looks like there is no line that contains "NODE_COORD_SECTION"
    */

    fprintf(stderr,"Tracing path file %s doesn't exist!\n",filename_tracing_path_points);
    error_handler((char *)"tracing_path_load");
 }

 for ( point_ind= 0 ; point_ind< point_nbr ; point_ind++ ) {

    /*
    Read the next line
    */

    char_ptr= fgets(line,MAXLINE,fp);

    /*
    Get the point coordinates from the line
    */

    sscanf(line,"%d%lg%lg",
     &index,
     &x,
     &y
    );

    if ( !(index == point_ind+1) ) { 
       error_handler((char *)"tracing_path_load"); 
    }

    point_arr[2*point_ind+0]= x; 
    point_arr[2*point_ind+1]= y;
 }

 fclose(fp);

 fp= fopen(filename_tracing_path_segments,"r");
 if ( fp == NULL ) {
    fprintf(stderr,"Tracing path segments file %s doesn't exist!\n",filename_tracing_path_segments);
    error_handler((char *)"tracing_path_load");
 }

 char_ptr= fgets(line,MAXLINE,fp);
 sscanf(line,"%d%d",
  &segment_nbr,
  &segment_nbr2);

 if ( !(segment_nbr2 == segment_nbr) ) {
    error_handler((char *)"tracing_path_load");
 }

 if ( !(segment_nbr == point_nbr) ) {
    error_handler((char *)"tracing_path_load");
 }

 for ( segment_ind= 0 ; segment_ind< segment_nbr ; segment_ind++ ) {

    /*
    Read the next line
    */

    char_ptr= fgets(line,MAXLINE,fp);

    sscanf(line,"%d%d%d",
     &point_ind0,
     &point_ind1,
     &dist_int
    );

    if ( segment_ind == 0 ) {
       first_point_ind0= point_ind0;
    }
    else {
       if ( !(point_ind0 == prev_point_ind1) ) {
          error_handler((char *)"tracing_path_load");
       }
    }

    x= point_arr[2*point_ind0+0];
    y= point_arr[2*point_ind0+1];

    /*
    Add the point to the tracing path
    */

    point= point_add(
     tracing_path,
     x,
     y
    );

    prev_point_ind1= point_ind1;
 }

 if ( !(point_ind1 == first_point_ind0) ) {
    error_handler((char *)"tracing_path_load");
 }

 fclose(fp);

 /*
 Free point_arr
 */

 if ( point_nbr > 0 )
  myFree(point_arr);

}
