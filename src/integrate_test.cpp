#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include "buffer2d_impl.hpp"
#include "model.hpp"
#include "raster.hpp"
#include "targa.hpp"
#include <catch.hpp>

TEST_CASE("integration/head", "[head, model, raster, buffer2d, targa]")
{
    Buffer2D<Targa::TargaFormat> buf(1000, 1000);
    Buffer2D<float> depth(1000, 1000);

    std::fstream file_normal("../obj/african_head_nm.tga");
    Buffer2D<Targa::TargaFormat> ntex = Targa::read(file_normal);

    std::fstream file_texture("../obj/african_head_diffuse.tga");
    Buffer2D<Targa::TargaFormat> tex = Targa::read(file_texture);
    depth.clear(-1.0f);

    std::fstream f("../obj/african_head.obj");
    auto head = LoadModel(f);

    for (auto tris : head) {

        auto& [a, b, c] = tris;
        auto vert = [](Attributes<glm::vec3, glm::vec3, glm::vec3>& ab) {
            glm::vec3& pos = std::get<0>(ab);
            float theta = -1.0f;
            glm::mat3x3 rot(
                glm::vec3(cos(theta), 0.0f, sin(theta)),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(-sin(theta), 0.0f, cos(theta)));
            pos = rot * pos;
        };

        vert(a);
        vert(b);
        vert(c);

        auto screenSpace = [&buf](Attributes<glm::vec3, glm::vec3, glm::vec3>& ab) {
            glm::vec3& r = std::get<0>(ab);
            r += glm::vec3(1.0f);
            r.x *= float(buf.width()) / 2.0f;
            r.y *= float(buf.height()) / 2.0f;
        };

        screenSpace(a);
        screenSpace(b);
        screenSpace(c);

        auto frag = [&ntex, &tex, &depth, &buf](Attributes<glm::vec3, glm::vec3, glm::vec3>& attrs) {
            glm::vec3 pos = std::get<0>(attrs);
            glm::vec3 uv = std::get<2>(attrs);
            glm::vec3 norm = glm::normalize(static_cast<glm::vec3>(ntex.get(uv.x * ntex.width(), uv.y * ntex.height())));
            glm::ivec3 ss = glm::round(pos);
            glm::vec3 lightDir = glm::normalize(glm::vec3(0.6f, 0.5f, 1.0f));
            float coefficient = glm::clamp(glm::dot(norm, lightDir), 0.0f, 1.0f);
            glm::vec3 texel = tex.get(uv.x * tex.width(), uv.y * tex.height());
            glm::vec3 color = (texel * coefficient);

            if (buf.isInBounds(ss.x, ss.y) && depth.get(ss.x, ss.y) < pos.z) {
                depth.set(ss.x, ss.y, pos.z);
                buf.set(ss.x, ss.y, Targa::TargaFormat(color));
            }
        };

        auto fragments = rasterTriangle(a, b, c);
        std::for_each(fragments.begin(), fragments.end(), frag);
    }

    std::ofstream tgaOut("images/head_model_lit.tga");
    tgaOut <<= buf;
}
