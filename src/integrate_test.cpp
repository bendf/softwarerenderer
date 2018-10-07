#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include "buffer2d_impl.hpp"
#include "model.hpp"
#include "raster.hpp"
#include "targa.hpp"
#include <catch.hpp>
#include <iomanip>
#include <iostream>
#include <numeric>

std::ostream& operator<<(std::ostream& stream, glm::vec3& v)
{
    return stream << "(" << v.x << "," << v.y << "," << v.z << ")";
}

template <typename VertexAttrs>
using Triangle = std::array<VertexAttrs, 3>;

template <typename VertexAttrs, typename FragAttrs, typename VertFunc, typename FragFunc>
void render(std::vector<Triangle<VertexAttrs>>& vIn, VertFunc vShader, FragFunc fShader, int vWidth, int vHeight)
{
    std::vector<Triangle<FragAttrs>> vOut(vIn.size());
    std::transform(vIn.begin(), vIn.end(), vOut.begin(), [vShader](auto& tri) {
        Triangle<FragAttrs> f; 
        std::transform(tri.begin(), tri.end(), f.begin(), vShader);
        return f;
    });

    for (auto& tri : vOut) {
        auto frags = rasterTriangle(tri[0], tri[1], tri[2], vWidth, vHeight);
        std::for_each(frags.begin(), frags.end(), fShader);
    }
}

TEST_CASE("integration/head", "[head, model, raster, buffer2d, targa]")
{
    Buffer2D<Targa::TargaFormat> buf(1000, 1000);
    Buffer2D<float> depth(1000, 1000);

    std::fstream file_normal("../obj/african_head_nm.tga");
    Buffer2D<Targa::TargaFormat> ntex = Targa::read(file_normal);

    std::fstream file_texture("../obj/african_head_diffuse.tga");
    Buffer2D<Targa::TargaFormat> tex = Targa::read(file_texture);

    //In this model, depth of 1.0 is closest to the camera, -1.0 furthest away. How, odd?
    std::fstream f("../obj/african_head.obj");
    auto head = LoadModel(f);

    std::vector<glm::vec3> lightPositions = {
        glm::vec3(0.0f, 0.0f, 10.0f),
        glm::vec3(0.0f, -30.0f, -10.0f),
    };

    int n = 5;
    for (int i = 0; i < n; i++) {
        std::cout << "Rendering image " << i << " of " << n << "..." << std::flush;
        float theta = i * (6.28f / n);
        glm::mat3x3 rot(
            glm::vec3(cos(theta), 0.0f, sin(theta)),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(-sin(theta), 0.0f, cos(theta)));

        auto vert = [rot](AttrTuple<glm::vec3, glm::vec3, glm::vec3> v) {
            std::get<0>(v) = rot * v.pos();
            return v;
        };

        using Fragment = std::pair<glm::ivec2, AttrTuple<glm::vec3, glm::vec3, glm::vec3>>;
        auto frag = [rot,lightPositions, &ntex, &tex, &depth, &buf](Fragment& frags) {
            auto [ss, attrs] = frags;
            glm::vec3 pos = attrs.pos();
            glm::vec3 uv = std::get<2>(attrs);
            Targa::TargaFormat normalTex = ntex.get(uv.x * (float)ntex.width(), uv.y * (float)ntex.height());
            glm::vec3 norm = glm::normalize((static_cast<glm::vec3>(normalTex) * glm::vec3(2.0f, 2.0, 2.0f)) - glm::vec3(1.0f));
            norm = rot * norm;
            float coefficient = std::accumulate(lightPositions.begin(), lightPositions.end(), 0.0f,
                [&pos, &norm](float f, glm::vec3 v) {
                    return f + glm::clamp(glm::dot(glm::normalize(v - pos), norm), 0.0f, 1.0f);
                });
            glm::vec3 texel = tex.get(uv.x * tex.width(), uv.y * tex.height());
            glm::vec3 color = texel * glm::clamp(coefficient, 0.0f, 1.0f);

            if (buf.isInBounds(ss.x, ss.y) && depth.get(ss.x, ss.y) < pos.z) {
                depth.set(ss.x, ss.y, pos.z);
                buf.set(ss.x, ss.y, Targa::TargaFormat(color));
            }
        };

        buf.clear({ 0, 0, 0 });
        depth.clear(-1.0f);

        using VAttributes = AttrTuple<glm::vec3, glm::vec3, glm::vec3>;
        render<VAttributes, VAttributes>(head, vert, frag, buf.width(), buf.height());

        std::for_each(lightPositions.begin(), lightPositions.end(), [&buf](glm::vec3 pos) {
            AttrTuple<glm::vec3> a(pos + glm::vec3(0.0f, 0.01f, 0.0f));
            AttrTuple<glm::vec3> b(pos + glm::vec3(0.01f, 0.0f, 0.0f));
            AttrTuple<glm::vec3> c(pos + glm::vec3(0.0f, 0.0f, 0.0f));
            auto lightFrags = rasterTriangle(a, b, c, buf.width(), buf.height());
            std::for_each(lightFrags.begin(), lightFrags.end(), [&buf](auto f) {
                auto [ss, pos] = f;
                buf.set(ss.x, ss.y, { 255, 255, 255 });
            });
        });

        std::stringstream ss;
        ss << "images/head_model_lit" << std::setfill('0') << std::setw(3) << i << ".tga";
        std::ofstream tgaOut(ss.str());
        tgaOut.exceptions(std::ios::failbit);
        tgaOut <<= buf;
        std::cout << "done!\n";
    }
}
