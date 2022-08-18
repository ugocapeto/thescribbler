#include "header.h"

void hermite_curve_recovering(
 arrayHeader *scribble_points,
 arrayHeader *seq_hermite
)

{

 int size;
 int i;
 double P1[2];
 double P2[2];
 double Pdummy1[2];
 double Pdummy2[2];
 double vec[2];
 double d;
 double _max;
 double _min;
 int steps;
 int t;
 double s;
 double s2;
 double s3;
 double h1;
 double h2;
 double h3;
 double h4;
 int j;
 double p[2];
 int maxId;
 double T1[2];
 double T2[2];
 int point;

 maxId= arrayMaxId(*scribble_points);
 size= maxId+1;

 _max = 0.0;
 _min = 10000000000.0;

 for ( i= 1 ; i< size ; i++ ) {
    if ( i == 1 ) {
       point_get(
        scribble_points,
        0,
        &P1[0],
        &P1[1]
       );
       point_get(
        scribble_points,
        1,
        &P2[0],
        &P2[1]
       );
       point_get(
        scribble_points,
        2,
        &Pdummy1[0],
        &Pdummy1[1]
       );
       vector_diff_2d(P2,P1,T1);
       vector_diff_2d(Pdummy1,P1,T2);
    }
    else if ( i == size-1 ) {
       point_get(
        scribble_points,
        i-1,
        &P1[0],
        &P1[1]
       );
       point_get(
        scribble_points,
        i,
        &P2[0],
        &P2[1]
       );
       point_get(
        scribble_points,
        i-2,
        &Pdummy1[0],
        &Pdummy1[1]
       );
       vector_diff_2d(P2,Pdummy1,T1);
       vector_diff_2d(P2,P1,T2);
    }
    else {
       point_get(
        scribble_points,
        i-1,
        &P1[0],
        &P1[1]
       );
       point_get(
        scribble_points,
        i,
        &P2[0],
        &P2[1]
       ); 
       point_get(
        scribble_points,
        i-2,
        &Pdummy1[0],
        &Pdummy1[1]
       );
       point_get(
        scribble_points,
        i+1,
        &Pdummy2[0],
        &Pdummy2[1]
       );
       vector_diff_2d(P2,Pdummy1,T1);
       vector_diff_2d(Pdummy2,P1,T2);
    }

    vector_diff_2d(P2,P1,vec);
    d = vector_norm_2d(vec);

    if ( d > _max )
     _max= d;
    if ( d < _min )
     _min= d;

    steps = 20;
    for ( t= 0 ; t< steps ; t++ ) {

       /*
       scale s to go from 0 to 1
       */

       s = (double)t / (double)steps;
       s2 = s*s;
       s3 = s2*s;

       /*
       calculate basis function 1
       */

       h1 =  2*s3 - 3*s2 + 1;

       /*
       calculate basis function 2
       */

       h2 = -2*s3 + 3*s2;

       /*
       calculate basis function 3
       */

       h3 =    s3 - 2*s2 + s;

       /*
       calculate basis function 4
       */

       h4 =    s3 -   s2;

       /*
       multiply and sum all funtions
       together to build the interpolated
       point along the curve 
       */

       for ( j= 0 ; j< 2 ; j++ ) {
          p[j] = h1*P1[j] +
                 h2*P2[j] +
                 h3*T1[j] +
                 h4*T2[j];
       }

       point= point_add(
        seq_hermite,
        p[0],
        p[1]
       );
    }
 }

}
