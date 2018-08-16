#ifndef RASTER_H
#define RASTER_H
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include "attributes.hpp"

glm::vec3 barycentricCoordinates2D(const glm::vec2 p0,
                                   const glm::vec2 p1, 
                                   const glm::vec2 p2, 
                                   const glm::vec2 p);

template<typename ...Ts>
std::vector<Attributes<Ts...>> rasterTriangle(const Attributes<Ts...>& a,
                                                const Attributes<Ts...>& b,
                                                const Attributes<Ts...>& c)
{
    std::vector<Attributes<Ts...>> tmp;

    glm::vec3 minPos = glm::min(glm::min(a.pos(),b.pos()),c.pos());
    glm::vec3 maxPos = glm::max(glm::max(a.pos(),b.pos()),c.pos());
    for(int x = minPos.x; x < maxPos.x; x++)
    {
        for(int y = minPos.y ; y < maxPos.y; y++)
        {
            auto bc = barycentricCoordinates2D(a.pos(), b.pos(), c.pos(), glm::vec2(x,y)); 
            auto inTriangle = [] (glm::vec3& bc) 
            { 
                return bc.x >= 0.0f && bc.x < 1.0f &&  bc.y >= 0.0f 
                    && bc.y < 1.0f &&  bc.z >= 0.0f && bc.y < 1.0f;
            };
            if(inTriangle(bc))
            {
                Attributes<Ts...> interpolated = (a*bc.x) + (b*bc.y) + (c*bc.z);
                tmp.push_back(interpolated);
            }
        }
    }
    return tmp;
}


#endif
