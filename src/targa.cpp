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

    TargaFormat operator+(const TargaFormat& lhs, const TargaFormat& rhs)
    {
        TargaFormat result;
        result.r = lhs.r + rhs.r;
        result.g = lhs.g + rhs.g;
        result.b = lhs.b + rhs.b;
        return result;
    }
    TargaFormat operator*(const TargaFormat& lhs, const float& rhs)
    {
       TargaFormat result; 
       result.r = lhs.r * rhs;
       result.b = lhs.g * rhs;
       result.g = lhs.b * rhs;
       return result;
    }

    TargaFormat toTargaFormat (glm::vec3 color)
    {
        TargaFormat t;
        t.r = color.r*255;
        t.b = color.b*255;
        t.g = color.g*255;
        return t;
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

