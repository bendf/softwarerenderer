#ifndef RASTER_H
#define RASTER_H
#include "attributes.hpp"
#include <array>
#include <glm/glm.hpp>
#include <vector>

glm::vec3 barycentricCoordinates2D(
    const glm::vec2 p0,
    const glm::vec2 p1,
    const glm::vec2 p2,
    const glm::vec2 p);

template <typename... Ts>
auto rasterTriangle(
    const Attributes<Ts...>& a,
    const Attributes<Ts...>& b,
    const Attributes<Ts...>& c,
    unsigned int width,
    unsigned int height)
{
    using Fragment = std::pair<glm::ivec2, Attributes<Ts...>>;
    std::vector<Fragment> fragments;

    auto screenSpace = [width,height](glm::vec3 v)
    {
        v+= glm::vec3(1.0f,1.0f,0.0f);
        v.x *= static_cast<float>(width)/2.0f;
        v.y *= static_cast<float>(height)/2.0f;
        return v;
    };

    glm::vec3 a_ss = screenSpace(a.pos());
    glm::vec3 b_ss = screenSpace(b.pos());
    glm::vec3 c_ss = screenSpace(c.pos());

    glm::vec3 minPos = glm::min(glm::min(a_ss,b_ss),c_ss);
    glm::vec3 maxPos = glm::max(glm::max(a_ss,b_ss),c_ss);

    for (int x = minPos.x; x < maxPos.x; x++) {
        for (int y = minPos.y; y < maxPos.y; y++) {
            auto bc = barycentricCoordinates2D(a_ss, b_ss, c_ss, glm::vec2(x, y));
            auto inTriangle = [](glm::vec3& bc) {
                return bc.x >= 0.0f && bc.x < 1.0f && bc.y >= 0.0f
                    && bc.y < 1.0f && bc.z >= 0.0f && bc.y < 1.0f;
            };
            if (inTriangle(bc)) {
                Attributes<Ts...> interpolated = (a * bc.x) + (b * bc.y) + (c * bc.z);
                fragments.push_back(std::make_pair(glm::ivec2(x,y),interpolated));
            }
        }
    }
    return fragments;
}

#endif
