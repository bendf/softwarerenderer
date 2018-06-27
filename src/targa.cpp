#include "targa.hpp"
#include <cmath>
#include <exception>
#include <sstream>
uint16_t Targa::getWidth() { return header.width; }
uint16_t Targa::getHeight() { return header.height; }

struct TargaHeader standardHeader(uint16_t width, uint16_t height)
{
    struct TargaHeader th = 
    {
        0, 0, 2, {0, 0, 0, 0, 0}, 0, 0, width, height, 24, 0
    };
    return th;
}


void Targa::write(std::ostream& stream)
{
    stream.write(reinterpret_cast<char const*>(&header), sizeof(header));
    stream.write(reinterpret_cast<char const*>(data), getWidth() * getHeight() * NUM_COLOR_COMPONENTS);
}


void Targa::getPixel(unsigned int x, unsigned int y, uint8_t& r, uint8_t& b, uint8_t& g) 
{
    if(x < 0 || x >= getWidth() || y < 0 || y >= getHeight())
    {
        std::stringstream err_stream;
        err_stream << "Read from pixel (" << x << "," << y 
                   << ") in image of size (" << getWidth() 
                   << "," << getHeight() << ")";
        throw std::out_of_range(err_stream.str());
    }

    unsigned int loc = NUM_COLOR_COMPONENTS * (y * getWidth() + x);
    b = data[loc+0];
    g = data[loc+1];
    r = data[loc+2];
 
}

void Targa::setPixel(unsigned int x, unsigned int y, float r, float g, float b)
{
    if(x < 0 || x >= getWidth() || y < 0 || y >= getHeight())
    {
        std::stringstream err_stream;
        err_stream << "Write to pixel (" << x << "," << y 
                   << ") in image of size (" << getWidth() 
                   << "," << getHeight() << ")";
        throw std::out_of_range(err_stream.str());
    }
    unsigned int loc = NUM_COLOR_COMPONENTS * (y * getWidth() + x);
    //Yep, BGR component order. Because RGB would be too simple
    data[loc+0] = (uint8_t)(b * 255);
    data[loc+1] = (uint8_t)(g * 255);
    data[loc+2] = (uint8_t)(r * 255);
}

void Targa::clear(float r, float b, float g) 
{
    for( int x = 0; x < getWidth(); x++)
    {
        for(int y =0; y < getHeight(); y++)
        {
            setPixel(x,y,r,b,g);
        }
    }
}
void Targa::drawLine(int x0, int y0, int x1, int y1, float r, float b, float g) 
{

}

Targa::Targa(uint16_t width, uint16_t height) 
{
   header = standardHeader(width, height); 
   data = new uint8_t[width*height*NUM_COLOR_COMPONENTS];
}

