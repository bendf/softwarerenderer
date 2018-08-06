#include "targa.hpp"
#include <fstream>
#include <cstdlib>

namespace Targa {

    TargaFormat::TargaFormat(glm::vec3 color)
    {
       r = static_cast<uint8_t>(color.r * 255);
       g = static_cast<uint8_t>(color.g * 255);
       b = static_cast<uint8_t>(color.b * 255);
    }

    TargaFormat::operator glm::vec3()
    {
        return glm::vec3(float(r)/255.0f, float(b)/255.0f, float(g)/255.0f);
    }

    TargaHeader::TargaHeader(uint16_t width, uint16_t height)
        : id_length(0),
          color_map_type(0),
          image_type(2),
          color_map_spec{0,0,0,0,0},
          ll_x(0),
          ll_y(0),
          width(width),
          height(height),
          bit_depth(24),
          image_descriptor(0)
    {
    }

    std::ostream& operator<<=(std::ostream& stream, const TargaHeader& header)
    {
        return stream.write(reinterpret_cast<char const*>(&header), sizeof(header));
    }

    std::ostream& operator<<=(std::ostream& stream, const Buffer2D<TargaFormat>& buffer) 
    {
       TargaHeader header(buffer.width(), buffer.height());
       return stream <<= header;//.write(reinterpret_cast<char const*>(buffer.rawPtr()), buffer.byteSize()); 
    }

    std::istream& operator>>=(std::istream& stream, TargaHeader& header)
    {
       return stream.read(reinterpret_cast<char*>(&header), sizeof(header)); 
    }

    
    Buffer2D<TargaFormat> read(std::istream& f) 
    {
        struct TargaHeader header;
        f >>= header;
        Buffer2D<TargaFormat> buffer(header.width, header.height);
        f.read(reinterpret_cast<char*>(buffer.rawPtr()), buffer.byteSize());
        return buffer;
    }
    
}

