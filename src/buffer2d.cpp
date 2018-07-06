#include "buffer2d.hpp"

void barycentricCoordinates(float x0, float y0,  float x1, float y1, float x2, float y2, float x, float y, float &u, float &v, float &w)
{

    float y12 = y1 - y2;
    float yy2 = y - y2;
    float xx2 = x - x2;
    float x02 = x0 - x2;
    float x21 = x2 - x1;
    float y02 = y0 - y2;

   float denom = y12*x02 + x21*y02; 
   u = (y12*xx2 + x21*yy2)/denom;
   v = (-y02*xx2 + x02*yy2)/denom;
   w = 1.0f - u - v;
}

