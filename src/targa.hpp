#include <cstdint>
#include <ostream>
#include <glm/glm.hpp>

typedef glm::vec3 Color;

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


class Targa 
{
    private:
        struct TargaHeader header;
        uint8_t* data;
        const unsigned int NUM_COLOR_COMPONENTS = 3;
    public:
        static const constexpr Color white = glm::vec3(1.0f,1.0f,1.0f);
        static const constexpr Color red = glm::vec3(1.0f,0.0f,0.0f);
        static const constexpr Color black = glm::vec3(0.0f,0.0f,0.0f);

        uint16_t getWidth();
        uint16_t getHeight();
        Targa(uint16_t width, uint16_t height);
        bool isInBounds(int x, int y);
        void write(std::ostream& stream);
        void setPixel(unsigned int x, unsigned int y, Color c);
        void drawLine(int x0, int y0, int x1, int y1, Color c);
        void clear(Color c);
        void getPixel(unsigned int x, unsigned int y, uint8_t& r, uint8_t& b, uint8_t& g);


};

