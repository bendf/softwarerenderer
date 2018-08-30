#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include "buffer2d_impl.hpp"
#include "model.hpp"
#include "raster.hpp"
#include "targa.hpp"
#include <catch.hpp>
#include <iostream>

std::ostream& operator<<(std::ostream& stream, glm::vec3& v)
{
    return stream << "(" << v.x << "," << v.y << "," << v.z << ")";
}

TEST_CASE("integration/head", "[head, model, raster, buffer2d, targa]")
{
    Buffer2D<Targa::TargaFormat> buf(1000, 1000);
    Buffer2D<float> depth(1000, 1000);

    std::fstream file_normal("../obj/african_head_nm.tga");
    Buffer2D<Targa::TargaFormat> ntex = Targa::read(file_normal);

    std::fstream file_texture("../obj/african_head_diffuse.tga");
    Buffer2D<Targa::TargaFormat> tex = Targa::read(file_texture);
    depth.clear(-1.0f);

    //In this model, depth of 1.0 is closest to the camera, -1.0 furthest away. How, odd?
    std::fstream f("../obj/african_head.obj");
    auto head = LoadModel(f);

    for (auto tris : head) {

        auto& [a, b, c] = tris;
        /*
        auto vert = [](Attributes<glm::vec3, glm::vec3, glm::vec3>& ab) {
            glm::vec3& pos = std::get<0>(ab);
            float theta = 0.0f;
            glm::mat3x3 rot(
                glm::vec3(cos(theta), 0.0f, sin(theta)),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(-sin(theta), 0.0f, cos(theta)));
            //pos = rot * pos;
        };

        vert(a);
        vert(b);
        vert(c);
        */

        using Fragment = std::pair<glm::ivec2, Attributes<glm::vec3,glm::vec3,glm::vec3>>;

        auto frag = [&ntex, &tex, &depth, &buf](Fragment& frags) {
            auto [ss,attrs] = frags;
            glm::vec3 pos = std::get<0>(attrs);
            glm::vec3 uv = std::get<2>(attrs);
            glm::vec3 norm = glm::normalize(static_cast<glm::vec3>(ntex.get(uv.x * (float)ntex.width(), uv.y * (float)ntex.height())));
            glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.75f);
            glm::vec3 lightDir = glm::normalize(lightPos - pos);
            float coefficient = glm::dot(lightDir,norm);//glm::length(lightDir); 
            glm::vec3 texel = tex.get(uv.x * tex.width(), uv.y * tex.height());
            glm::vec3 color = glm::vec3(1.0f) * glm::clamp(coefficient, 0.0f, 1.0f); 

            if (buf.isInBounds(ss.x, ss.y) && depth.get(ss.x, ss.y) < pos.z) {
                depth.set(ss.x, ss.y, pos.z);
                buf.set(ss.x, ss.y, Targa::TargaFormat(color));
            }
        };

        auto fragments = rasterTriangle(a, b, c, buf.width(), buf.height());
        std::for_each(fragments.begin(), fragments.end(), frag);
    }

    std::ofstream tgaOut("images/head_model_lit.tga");
    tgaOut <<= buf;
}
