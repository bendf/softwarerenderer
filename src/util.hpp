#include "buffer2d.hpp"
#include "util.hpp"

glm::vec3 barycentricCoordinates(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p)
{
    // p = up0 + vp1 + wp2
    // w = 1 - u - v
    // magnitude of cross product to calculate parallelogram, half it for triangle size, but we are diving by denom, which is also a triangle, so factor out the divide by 2
    glm::vec3 triNormal = glm::cross(p1 - p0, p2 - p0);
    float denom = glm::length(triNormal);
    glm::vec3 uNorm = glm::cross(p1 - p, p2 - p);
    glm::vec3 vNorm = glm::cross(p2 - p, p0 - p);
    float u =  sgn(glm::dot(uNorm,triNormal)) *  glm::length(uNorm)/denom;
    float v = sgn(glm::dot(vNorm,triNormal)) * glm::length(vNorm)/denom;
    float w = 1.0f - (u + v);

    return glm::vec3(u,v,w);
}


glm::vec3 clipToViewport(glm::vec3 p, int width, int height)
{
    return (p + glm::vec3(1.0f, 1.0f,0.0f)) * glm::vec3(float(width)/2.0f, float(height)/2.0f, 1.0f);
}

template<typename T>
unsigned int countBufferValues(Buffer2D<T>& buffer, T value)
{
    unsigned int acc = 0;
    for(int x = 0; x < buffer.getWidth(); x++)
    {
        for(int y = 0; y < buffer.getHeight(); y++)
        {
          acc += (buffer.get(x,y) == value);  
        }
    }
    return acc;
}



template<typename T>
void drawLine(int x0, int y0, int x1, int y1, T value, Buffer2D<T>* buffer)
{
  bool steep = false;
  if(std::abs(y1 - y0) > std::abs(x1 - x0))
  {
      steep = true;
      std::swap(x0,y0);
      std::swap(x1,y1);
  }
  if(x1 < x0)
  {
    std::swap(x0,x1);
    std::swap(y0,y1);
  }

  int deltaX = (x1 - x0);
  int deltaY = (y1 - y0);
  for(int x = x0, y = y0, error = 0; x <= x1; x++,error+=2*deltaY)  
  {
    if(std::abs(error) > deltaX)
    {
        error -= sgn(deltaY)*2*deltaX;
        y+=sgn(deltaY);
    }
    if(buffer->isInBounds(x,y))
    {
        if(steep)
        {
            set(y,x,value);
        }
        else 
        {
            set(x,y,value);
        }
    }
  }
}
