#include "targa.hpp"
#include <cmath>
#include <exception>
#include <sstream>
#include <cstdlib>
#include <algorithm>

TargaFormat toTargaFormat(FloatColor fc)
{
    TargaFormat tc;
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
    stream.write(reinterpret_cast<char const*>(data), width * height * sizeof(TargaFormat));
}


TargaFormat Targa::getPixel(PixelCoord p) 
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
        data[loc] = toTargaFormat(color);
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

glm::vec3 toBarycentric(glm::vec2 p, glm::vec2 A, glm::vec2 B, glm::vec2 C)
{
    glm::vec3 barycentric;
    float det = (B.y - C.y)*(A.x - C.x) + (C.x - B.x)*(A.y - C.y);
    barycentric.x = ((B.y - C.y)*(p.x - C.x) + (C.x - B.x)*(p.y - C.y))/det; 
    barycentric.y = ((C.y - A.y)*(p.x - C.x) + (A.x - C.x)*(p.y - C.y))/det; 
    barycentric.z = 1 - (barycentric.x + barycentric.y);
    return barycentric;
     
}
bool isInTriangle(glm::vec2 p, glm::vec2 A, glm::vec2 B, glm::vec2 C)
{
     
    glm::vec3 bary = toBarycentric(p, A, B, C);
    return bary.x >= 0 && bary.y >=0 && bary.z >= 0;
    return true;
}

void Targa::drawTriangle(PixelCoord p0, PixelCoord p1, PixelCoord p2, FloatColor color)
{

    glm::ivec2 topRight = glm::ivec2(std::max({p0.x,p1.x,p2.x}),std::max({p0.y,p1.y,p2.y}));
    glm::ivec2 botLeft = glm::ivec2(std::min({p0.x,p1.x,p2.x}),std::min({p0.y,p1.y,p2.y}));

    for(int x = botLeft.x; x<=topRight.x;x++)
    {
        for(int y = botLeft.y; y <=topRight.y; y++)
        {
            glm::ivec2 p(x,y);
            if(isInTriangle(p,p0,p1,p2))
            {
                setPixel(p,color);
            }
        }
    }
    
}

bool isSteep(glm::ivec2 v)
{
    return std::abs(v.y) > std::abs(v.x);
}
void Targa::drawLine(PixelCoord p0, PixelCoord p1, FloatColor c) 
{
    bool transpose = false;
    if(isSteep(p1-p0))
    {
        std::swap(p0.x,p0.y);
        std::swap(p1.x,p1.y);
        transpose = true;
    }

    if(p1.x < p0.x)
    {
        std::swap(p0,p1);
    }

    float grad = p1.x == p0.x ? 0.0f :float(p1.y - p0.y)/float(p1.x - p0.x);
    for(int x=p0.x; x <= p1.x; x++)
    {
        glm::ivec2 p(x,p0.y + grad*(x-p0.x));
        if(transpose)
        {
            std::swap(p.x,p.y);
        }
        setPixel(p,c);

    }
     
}
/*
    bool transpose = false;
    if(p0 == p1)
    {
        setPixel(p0,c);
        return;
    }

    float grad = float(p1.y - p0.y) / float(p1.x - p0.x);   

    //Check for division by zero
    if(p1.x == p0.x || std::abs(grad) > .0f)
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
*/

PixelCoord Targa::fromClip(ClipCoord cc)
{
    glm::ivec2 pc;
    pc.x = (cc.x + 1.0f) * (width / 2.0f);
    pc.y = (cc.y + 1.0f) * (height / 2.0f);
    return pc;
}

Targa::Targa(uint16_t width, uint16_t height) 
{
   data = new TargaFormat[width*height];
   this->width = width;
   this->height = height;
}
Targa::~Targa()
{
    delete data;
}

