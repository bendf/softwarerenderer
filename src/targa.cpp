#include "targa.hpp"
#include <cmath>
#include <exception>
#include <sstream>
#include<cstdlib>

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


const constexpr Color Targa::white;
const constexpr Color Targa::black;
const constexpr Color Targa::red;
const constexpr Color Targa::green;

void Targa::write(std::ostream& stream)
{
    stream.write(reinterpret_cast<char const*>(&header), sizeof(header));
    stream.write(reinterpret_cast<char const*>(data), getWidth() * getHeight() * NUM_COLOR_COMPONENTS);
}


void Targa::getPixel(unsigned int x, unsigned int y, uint8_t& r, uint8_t& b, uint8_t& g) 
{
    if(!isInBounds(x,y))
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

void Targa::setPixel(int x, int y, Color c)
{
    if(!isInBounds(x,y))
    {
        std::stringstream err_stream;
        err_stream << "Write to pixel (" << x << "," << y 
                   << ") in image of size (" << getWidth() 
                   << "," << getHeight() << ")";
        throw std::out_of_range(err_stream.str());
    }
    unsigned int loc = NUM_COLOR_COMPONENTS * (y * getWidth() + x);
    //Yep, BGR component order. Because RGB would be too simple
    data[loc+0] = (uint8_t)(c.b * 255);
    data[loc+1] = (uint8_t)(c.g * 255);
    data[loc+2] = (uint8_t)(c.r * 255);
}

bool Targa::isInBounds(int x, int y)
{
    return  x >= 0 && x < getWidth() && y >= 0 && y < getHeight();

}

void Targa::clear(Color c) 
{
    for( int x = 0; x < getWidth(); x++)
    {
        for(int y =0; y < getHeight(); y++)
        {
            setPixel(x,y,c);
        }
    }
}

glm::vec3 randomColor()
{
    float r,g,b;
    r = float(rand()) / float(RAND_MAX);
    g = float(rand()) / float(RAND_MAX);
    b = float(rand()) / float(RAND_MAX);
    return glm::vec3(r,g,b);
}

void Targa::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color c)
{
    glm::vec3 p0(x0,y0,0);
    glm::vec3 p1(x1,y1,0);
    glm::vec3 p2(x2,y2,0);

    glm::vec3 norm = glm::cross(p1-p0,p2-p0);
    if(norm.z < 0.0f)
    {
        return;
    }

    if(y1 > y0)
    {
        std::swap(y1,y0);
        std::swap(x1,x0);
    }
    if(y2 > y0)
    {
        std::swap(y0,y2);
        std::swap(x0,x2);
    }
    if(y2 > y1)
    {
        std::swap(y1,y2);
        std::swap(x1,x2);
    }

    //avoid division by zero
    if(y0 == y1)
    {
        drawLine(x0,y0, x1,y1,Targa::green);
    } 
    else 
    {
        glm::vec3 col = randomColor(); 
        for(int y = y0; y >=y1; y--)
        {

            float gradStart = float(x1 - x0) / float(y1 - y0);
            int xStart = x0 + gradStart * (y - y0);
            float gradEnd = float(x2 - x0) / float(y2 - y0);
            int xEnd = x0 +  gradEnd * (y - y0);
            drawLine(xStart,y, xEnd, y, col);
        }
    }

    //Avoid division by zero
    if(y1 == y2)
    {
        drawLine(x1,y1, x2,y2,Targa::green);
    }
    else 
    {

        glm::vec3 col = randomColor();
        for(int y = y2; y <=y1; y++)
        {
            float gradStart = float(x1 - x2) / float(y1 - y2);
            int xStart = x2 + gradStart * (y - y2);
            float gradEnd = float(x0 - x2) / float(y0 - y2);
            int xEnd = x2 +  gradEnd * (y - y2);
            drawLine(xStart,y, xEnd, y,col );
        }
    } 

    
}
void Targa::drawLine(int x0, int y0, int x1, int y1, Color c) 
{
    bool transpose = false;
    if((x0 == x1) && (y0 == y1))
    {
        if(isInBounds(x0,y0))
        {
            setPixel(x0,y0,c);
        }
        return;
    }

    float grad = float(y1 - y0) / float(x1 - x0);   

    //Check for division by zero
    if(x1 == x0 || std::abs(grad) > 1.0f)
    {
        std::swap(x0,y0);
        std::swap(x1,y1);
        grad = float(y1 - y0) / float(x1 - x0);   
        transpose = true;
    }

    if(x1 < x0) {
        std::swap(x0,x1);
        std::swap(y0,y1);
    }

    for(int cx = x0; cx <= x1; cx++) 
    {
        int cy = y0 + (grad * (cx - x0));
        
        if(transpose)
        {
            if(isInBounds(cy,cx)) 
            {
                setPixel(cy,cx, c);
            }
        }
        else
        {
            if(isInBounds(cx,cy))
            {
                setPixel(cx,cy, c);
            }
        }
    }

}

Targa::Targa(uint16_t width, uint16_t height) 
{
   header = standardHeader(width, height); 
   data = new uint8_t[width*height*NUM_COLOR_COMPONENTS];
}

