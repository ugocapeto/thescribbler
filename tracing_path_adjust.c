#include "header.h"

void tracing_path_adjust(
 arrayHeader *tracing_path
)

/*
StippleGen, for some reason,
resizes the input image to 800x600 and then resizes the output to 3200x800
Check https://github.com/evil-mad/stipplegen/blob/master/StippleGen/StippleGen.pde
*/

/*
For "The Scribbler" to work with StippleGen,
the input image must be resized to 800x600 (using gimp) and the points must be repositioned
so that the tracing path matches the (resized) input image
*/

{

 int mainwidth;
 int mainheight;
 double SVGscale;
 double xOffset;
 double yOffset;
 int maxId;
 int point_nbr;
 int point;
 double x;
 double y;
 double xTemp;
 double yTemp;

 mainwidth = 800;
 mainheight = 600;
 SVGscale = (double)mainwidth / (double)mainheight;
 xOffset = 1600.0 - (SVGscale * (double)mainwidth  / 2.0);
 yOffset =  400.0 - (SVGscale * (double)mainheight / 2.0);

 maxId= arrayMaxId(*tracing_path);
 point_nbr= maxId+1;

 for ( point= 0 ; point< point_nbr ; point++ ) {

    point_get(
     tracing_path,
     point,
     &x,
     &y
    );

    xTemp= x;
    yTemp= y;

    x= (xTemp - xOffset)/SVGscale;
    y= (yTemp - yOffset)/SVGscale;

    point_set(
     tracing_path,
     point,
     x,
     y
    );
 }

}
