#include <string>
#include <stdexcept> 
#include <sstream>
#include <memory>
#include <algorithm>

template<typename T>
Buffer2D<T>::Buffer2D(int width, int height) : width{width}, height{height}
{
    if(width <=0 || height <=0)
    {
        std::stringstream ss;
        ss << "Can't initliazer buffer of size (" << width << "," << height << ")";
        throw std::invalid_argument(ss.str());
    }
    data = std::make_unique<T[]>(width*height);
}

template<typename T>
size_t Buffer2D<T>::getByteSize()
{
    return width * height * sizeof(T);
}

template<typename T>
void Buffer2D<T>::boundsAssert(int x, int y)
{
    if(!isInBounds(x, y))
    {
        std::stringstream ss;
        ss << "Attempt to access location (" << x << "," << y
           << ") in buffer of size (" << width << "," << height << ")";
        throw std::out_of_range(ss.str());
    }

}
template<typename T>
T Buffer2D<T>::get(int x, int y)
{
    boundsAssert(x,y);
    return data[(y*width) + x];
}

template<typename T>
void Buffer2D<T>::set(int x, int y, T value)
{
    boundsAssert(x,y);
    data[(y*width) + x] = value;
}

template<typename T>
void Buffer2D<T>::clear(T value)
{
    for(int i = 0; i < width*height; i++)
    {
        data[i] = value;
    }
}

template<typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}


template<typename T>
void Buffer2D<T>::drawLine(int x0, int y0, int x1, int y1, T value)
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
    if(isInBounds(x,y))
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


template<typename T>
void Buffer2D<T>::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, T value)
{
   int x_min = std::min({x0,x1,x2}); 
   int y_min = std::min({y0,y1,y2});
   int x_max = std::max({x0,x1,x2}); 
   int y_max = std::max({y0,y1,y2}); 
   for(int x = x_min; x <= x_max; x++)
   {
        for(int y = y_min; y <= y_max; y++)
        {
            if(isInBounds(x,y))
            {
                float u,v,w;
                barycentricCoordinates(x0,y0, x1,y1, x2,y2, x,y, u,v,w);
                if((u >= 0) && (v >= 0) && (w >= 0))
                {
                    set(x,y,value);
                }
            }
        }
   }

}
