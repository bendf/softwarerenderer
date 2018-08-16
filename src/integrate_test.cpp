#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include "buffer2d_impl.hpp"
#include "targa.hpp"
#include "model.hpp"
#include "raster.hpp"

TEST_CASE("integration/head", "[head, model, raster, buffer2d, targa]")
{
    Buffer2D<Targa::TargaFormat> buf(1000,1000);

    std::fstream f("../obj/african_head.obj");
    Model head(f);

    for(auto attrs : head)
    {
        auto& [a,b,c] = attrs;
        auto screenSpace = [&buf](Attributes<glm::vec3,glm::vec3,glm::vec3>& ab)
        {
            glm::vec3& r = std::get<0>(ab);
            r+=glm::vec3(1.0f);
            r.x*= float(buf.width())/2.0f;
            r.y*= float(buf.height())/2.0f;
        };

        screenSpace(a);
        screenSpace(b);
        screenSpace(c);

        auto frag = [&buf] (Attributes<glm::vec3,glm::vec3,glm::vec3>& attrs)
        {
            glm::vec3 pos = std::get<0>(attrs);
            glm::vec3 norm = std::get<1>(attrs);
            glm::ivec3 ss= glm::round(pos);
            float coefficient = glm::dot(norm, glm::vec3(0.0f,0.0f,1.0f)); 
            glm::vec3 color = glm::vec3(1.0f);
            if(coefficient > 0.0f)
            {
                if(buf.isInBounds(ss.x,ss.y))
                {
                    buf.set(ss.x,ss.y, Targa::TargaFormat(color * coefficient));
                }
            }
        };

        auto fragments = rasterTriangle(a,b,c);
        std::for_each(fragments.begin(), fragments.end(), frag);

    }
    
    std::ofstream tgaOut("images/head_model_red.tga");
    tgaOut <<= buf;

}

TEST_CASE("integration/quad", "[quad, model, raster, buffer2d, targa]")
{
    Buffer2D<Targa::TargaFormat> buf(1000,1000);

    std::fstream f("../obj/quad.obj");
    Model quad(f);

    for(auto attrs : quad)
    {
        auto& [a,b,c] = attrs;
        auto screenSpace = [&buf](Attributes<glm::vec3,glm::vec3,glm::vec3>& ab)
        {
            glm::vec3& r = std::get<0>(ab);
            r+=glm::vec3(1.0f);
            r.x*= float(buf.width())/2.0f;
            r.y*= float(buf.height())/2.0f;
        };

        screenSpace(a);
        screenSpace(b);
        screenSpace(c);

        for(auto interpolated : rasterTriangle(a,b,c))
        {
            glm::vec3 pos = interpolated.pos();

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

