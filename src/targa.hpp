#include <cstdint>
#include <glm/glm.hpp>
#include "buffer2d.hpp"

namespace Targa {

    struct TargaFormat
    {
        uint8_t b,g,r;
        TargaFormat() : TargaFormat(0,0,0) {}
        TargaFormat(uint8_t r, uint8_t g, uint8_t b) : b(b), g(g), r(r) {}
        TargaFormat(glm::vec3 color);
        operator glm::vec3();
    };

    struct TargaHeader
    {
        //Length of id section of image file (0 bytes for our purposes)
        uint8_t id_length; 
        //color map type (0 or 1, probably)
        uint8_t color_map_type; 
        //Probably 2 for uncompressed true color
        uint8_t image_type;  
        //Probably empty (no color map)
        uint8_t color_map_spec[5]; 
        //2 bytes -- lower left corner x coord?
        uint16_t ll_x;
        //2 bytes -- lower left corner y coord?
        uint16_t ll_y;
        //2 bytes -- width
        uint16_t width;
        //2 bytes -- height
        uint16_t height;
        //1 bytes -- pixel depth -- 24?
        uint8_t bit_depth;
        //1 bytes -- descriptor , alpha channel depth and direction?? wtf stick with 0
        uint8_t image_descriptor;
        TargaHeader() : TargaHeader(0,0) {}
        TargaHeader(uint16_t width, uint16_t height);

    };

    std::ostream& operator<<=(std::ostream& stream, const TargaHeader& header);
    std::ostream& operator<<=(std::ostream& stream, const Buffer2D<TargaFormat>& buffer);
    std::istream& operator>>=(std::istream& stream, TargaHeader& header);

    Buffer2D<TargaFormat> read(std::istream& f);

}

