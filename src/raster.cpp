#include "raster.hpp"


glm::vec3 barycentricCoordinatesOrtho(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec2 p)
{
  float y2_y3 = p1.y - p2.y;
  float x1_x3 = p0.x - p2.x;
  float x3_x2 = p2.x - p1.x;
  float y1_y3 = p0.y - p2.y;
  float x_x3 = p.x - p2.x;
  float y_y3 = p.y - p2.y;

  float denom = (y2_y3 * x1_x3) + (x3_x2*y1_y3);
  float u = ((y2_y3 * x_x3) + (x3_x2*y_y3))/denom;
  float v = ((-y1_y3 * x_x3) + (x1_x3*y_y3))/denom;
  float w = 1.0f - (u + v);

  return glm::vec3(u,v,w);

}



