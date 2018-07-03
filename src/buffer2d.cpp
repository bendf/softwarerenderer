#include <string>
#include <stdexcept>
#include <memory>
#include "buffer2d.hpp"
using namespace std;

template<typename T>
Buffer2D<T>::Buffer2D(int width, int height) : width{width}, height{height}
{
    if(width <=0 || height <=0)
    {
        string msg = string("Can't initalize buffer of width") + to_string(width) 
            + string(" and height ") + to_string(height);
        throw std::invalid_argument(msg);
    }
    data = make_unique<T[]>(width*height);
}


template<typename T>
T Buffer2D<T>::get(int x, int y)
{
}


