#ifndef BUFFER_2D_H
#define BUFFER_2D_H
#include <cstdlib>
#include <memory>
#include <sstream>

template<typename T>
class Buffer2D
{
    private:
    std::unique_ptr<T[]> data;
    int _width, _height;

    public:
    Buffer2D(int width, int height);

    bool isInBounds(int x, int y) const { return x >=0 && x < _width && y >= 0 && y < _height;}

    int width() const { return _width; }
    int height() const { return _height; }
    size_t byteSize() const { return _width * _height * sizeof(T);}
    T* rawPtr() const { return data.get(); }

    T get(int x, int y) const;
    void set(int x, int y, T value);
    void clear(T value);

};

class buffer2d_out_of_range : public std::exception 
{
    std::string msg;
    public:
    buffer2d_out_of_range(int x, int y, int width, int height)  
    {
        std::stringstream ss; 
        ss << "Attempt to access location (" << x << "," << y
           << ") in buffer of size (" << width << "," << height << ")";
        msg = ss.str(); 
    }

    char const * what() 
    {
        return msg.c_str();
    }
};

template<typename T>
Buffer2D<T>::Buffer2D(int width, int height) : _width{width} , _height{height}
{
    if(_width <=0 || _height <=0)
    {
        std::stringstream ss;
        ss << "Can't initliazer buffer of size (" << width << "," << height << ")";
        throw std::invalid_argument(ss.str());
    }
    data = std::make_unique<T[]>(width*height);
}


template<typename T>
T Buffer2D<T>::get(int x, int y) const
{
    if(isInBounds(x,y))
    {
        return data[(y*_width) + x];
    }
    else 
    {
        throw buffer2d_out_of_range(x,y, _width,_height);
    }
}

template<typename T>
void Buffer2D<T>::set(int x, int y, T value)
{
    if(isInBounds(x,y))
    {
        data[(y*_width)+x] = value;
    } 
    else 
    {
        throw buffer2d_out_of_range(x,y,_width,_height);
    }
}

template<typename T>
void Buffer2D<T>::clear(T value)
{
    for(int i = 0; i < _width*_height; i++)
    {
        data[i] = value;
    }
}

#endif

