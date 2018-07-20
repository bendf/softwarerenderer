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

    void write(const char * filename, Buffer2D<TargaFormat> & buffer)
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

