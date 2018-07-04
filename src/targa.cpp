#include "targa.hpp"
#include <fstream>
#include <cstdlib>

namespace Targa {

    struct TargaHeader generateHeader(uint16_t width, uint16_t height)
    {
        struct TargaHeader header = 
        {
            0, 0, 2, {0, 0, 0, 0, 0}, 0, 0, width, height, 24, 0
        };
        return header;
    }

    bool operator==(const TargaFormat& lhs, const TargaFormat& rhs)
    {
        return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
    }
    void write(const char * filename, Buffer2D<TargaFormat>& buffer)
    {
        std::ofstream f(filename);
        f.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        struct TargaHeader header = generateHeader(buffer.getWidth(), buffer.getHeight());
        f.write(reinterpret_cast<char const*>(&header), sizeof(header));
        f.write(reinterpret_cast<char const*>(buffer.rawData()), buffer.getByteSize());

    }
    Buffer2D<TargaFormat> read(const char * filename) 
    {
        std::ifstream f(filename);
        f.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        struct TargaHeader header;
        f.read(reinterpret_cast<char*>(&header), sizeof(header)); 
        Buffer2D<TargaFormat> buffer(header.width, header.height);
        f.read(reinterpret_cast<char*>(buffer.rawData()), buffer.getByteSize());
        return buffer;
    }
}

/*
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

void Targa::drawTriangle(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec3 color)
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
void Targa::drawLine(glm::vec2 p0, glm::vec2 p1, glm::vec3 c) 
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

*/
