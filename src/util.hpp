#include "buffer2d.hpp"
template<typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
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
