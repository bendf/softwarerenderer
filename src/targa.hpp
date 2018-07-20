#include <cstdint>
#include <glm/glm.hpp>
#include "buffer2d.hpp"

namespace Targa {

    struct TargaFormat
    {
        uint8_t b,g,r;
        operator glm::vec3()
        {
            return glm::vec3(float(r)/255.0f, float(b)/255.0f, float(g)/255.0f);
        }
    };

    bool operator==(const TargaFormat& lhs, const TargaFormat& rhs);

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
    };

    TargaHeader generateHeader(uint16_t width, uint16_t height);

    void write(const char* filename, Buffer2D<TargaFormat>& buffer);
    Buffer2D<TargaFormat> read(const char* filename);
}

