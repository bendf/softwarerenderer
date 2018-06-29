#include "targa.hpp"
#include <cmath>
#include <exception>
#include <sstream>
#include<cstdlib>

TargaColor toTargaColor(FloatColor fc)
{
    TargaColor tc;
    tc.b = (uint8_t)(fc.b * 255);
    tc.g = (uint8_t)(fc.g * 255);
    tc.r = (uint8_t)(fc.r * 255);
    return tc;
}

uint16_t Targa::getWidth() { return width; }
uint16_t Targa::getHeight() { return height; }

struct TargaHeader Targa::generateHeader()
{
    struct TargaHeader header = 
    {
        0, 0, 2, {0, 0, 0, 0, 0}, 0, 0, width, height, 24, 0
    };
    return header;
}

const constexpr FloatColor Targa::white;
const constexpr FloatColor Targa::black;
const constexpr FloatColor Targa::red;
const constexpr FloatColor Targa::green;

void Targa::write(std::ostream& stream)
{
    struct TargaHeader header = generateHeader();
    stream.write(reinterpret_cast<char const*>(&header), sizeof(header));
    stream.write(reinterpret_cast<char const*>(data), width * height * sizeof(TargaColor));
}


TargaColor Targa::getPixel(PixelCoord p) 
{
    if(!isInBounds(p))
    {
        std::stringstream err_stream;
        err_stream << "Read from pixel (" << p.x << "," << p.y 
                   << ") in image of size (" << getWidth() 
                   << "," << getHeight() << ")";
        throw std::out_of_range(err_stream.str());
    }
    unsigned int loc = (width*p.y) + p.x;
    return data[loc];
 
}

void Targa::setPixel(PixelCoord p, FloatColor color)
{
    if(isInBounds(p))
    {
        unsigned int loc = (width * p.y) + p.x;
        //Yep, BGR component order. Because RGB would be too simple
        data[loc] = toTargaColor(color);
    }
}

bool Targa::isInBounds(PixelCoord p)
{
    return  p.x >= 0 && p.x < getWidth() &&
            p.y >= 0 && p.y < getHeight();
}

void Targa::clear(FloatColor c) 
{
    for( int x = 0; x < getWidth(); x++)
    {
        for(int y =0; y < getHeight(); y++)
        {
            setPixel(glm::ivec2(x,y),c);
        }
    }
}

FloatColor randomColor()
{
    float r,g,b;
    r = float(rand()) / float(RAND_MAX);
    g = float(rand()) / float(RAND_MAX);
    b = float(rand()) / float(RAND_MAX);
    return glm::vec3(r,g,b);
}

void Targa::drawTriangle(PixelCoord p0, PixelCoord p1, PixelCoord p2, FloatColor c)
{

    if(p1.y > p2.y)
    {
        std::swap(p0,p1);
    }
    if(p2.y > p0.y)
    {
        std::swap(p0,p2);
    }
    if(p2.y > p1.y)
    {
        std::swap(p1,p2);
    }

    FloatColor col = randomColor();
    //avoid division by zero
    if(p0.y == p1.y)
    {
        drawLine(p0, p1,Targa::green);
    } 
    else 
    {
        for(int y = p0.y; y >=p1.y; y--)
        {

            float gradStart = float(p1.x - p0.x) / float(p1.y - p0.y);
            int xStart = p0.x + gradStart * (y - p0.y);
            float gradEnd = float(p2.x - p0.x) / float(p2.y - p0.y);
            int xEnd = p0.x +  gradEnd * (y - p0.y);
            drawLine(glm::ivec2(xStart,y), glm::ivec2(xEnd,y), col);
        }
    }

    //Avoid division by zero
    if(p1.y == p2.y)
    {
        drawLine(p1, p2, Targa::green);
    }
    else 
    {

        for(int y = p2.y; y <=p1.y; y++)
        {
            float gradStart = float(p1.x - p2.x) / float(p1.y - p2.y);
            int xStart = p2.x + gradStart * (y - p2.y);
            float gradEnd = float(p0.x - p2.x) / float(p0.y - p2.y);
            int xEnd = p2.x +  gradEnd * (y - p2.y);
            drawLine(glm::ivec2(xStart,y), glm::ivec2(xEnd, y),col );
        }
    } 

    
}
void Targa::drawLine(PixelCoord p0, PixelCoord p1, FloatColor c) 
{
    bool transpose = false;
    if(p0 == p1)
    {
        setPixel(p0,c);
        return;
    }

    float grad = float(p1.y - p0.y) / float(p1.x - p0.x);   

    //Check for division by zero
    if(p1.x == p0.x || std::abs(grad) > 1.0f)
    {
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
        grad = float(p1.y - p0.y) / float(p1.x - p0.x);   
        transpose = true;
    }

    if(p1.x < p0.x) {
        std::swap(p0,p1);
    }

    for(int cx = p0.x; cx <= p1.x; cx++) 
    {
        int cy = p0.y + (grad * (cx - p0.x));
        
        if(transpose)
        {
                setPixel(glm::ivec2(cy,cx), c);
        }
        else
        {
                setPixel(glm::ivec2(cx,cy), c);
        }
    }

}

PixelCoord Targa::fromClip(ClipCoord cc)
{
    glm::ivec2 pc;
    pc.x = (cc.x + 1.0f) * (width / 2.0f);
    pc.y = (cc.y + 1.0f) * (height / 2.0f);
    return pc;
}

Targa::Targa(uint16_t width, uint16_t height) 
{
   data = new TargaColor[width*height];
   this->width = width;
   this->height = height;
}
Targa::~Targa()
{
    delete data;
}

