#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include "buffer2d_impl.hpp"
#include "targa.hpp"
#include "model.hpp"
#include "raster.hpp"


TEST_CASE("integration/quad", "[quad, model, raster, buffer2d, targa]")
{
    Buffer2D<Targa::TargaFormat> buf(100,100);

    std::fstream f("../obj/quad.obj");
    Model quad(f);

    for(auto attrs : quad)
    {
        glm::vec3 p0 = quad.positions.at(std::get<0>(attrs).pos -1);
        p0 += glm::vec3(1.0f);
        p0.x*= 100.0f/2.0f;
        p0.y*= 100.0f/2.0f;
        glm::vec3 p1 = quad.positions.at(std::get<1>(attrs).pos -1);
        p1 += glm::vec3(1.0f);
        p1.x*= 100.0f/2.0f;
        p1.y*= 100.0f/2.0f;
        glm::vec3 p2 = quad.positions.at(std::get<2>(attrs).pos -1);
        p2 += glm::vec3(1.0f);
        p2.x*= 100.0f/2.0f;
        p2.y*= 100.0f/2.0f;

        PosAttr a{p0};
        PosAttr b{p1};
        PosAttr c{p2};
        for(auto interpolated : rasterTriangle(a,b,c))
        {
            glm::vec3 pos = interpolated.pos;

            int x = std::round(pos.x);
            int y = std::round(pos.y);
            
            if(buf.isInBounds(x,y))
            {
                buf.set(x,y, {255,0,0});
            }

        }

    }
    
    std::ofstream tgaOut("images/quad_model.tga");
    tgaOut <<= buf;

}
