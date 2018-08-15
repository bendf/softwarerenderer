#ifndef RASTER_H
#define RASTER_H
#include <glm/glm.hpp>
#include <vector>
#include <array>

glm::vec3 barycentricCoordinates2D(const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2, const glm::vec2 p);


struct PosAttr
{
    glm::vec3 pos;
    PosAttr operator*(const float o) const {return PosAttr{pos * o};}
    PosAttr operator+(const PosAttr& o) const { return PosAttr{pos + o.pos};}
};



template<typename T>
std::vector<T> rasterTriangle(const T& a, const T& b, const T& c)
{
    std::vector<T> tmp;
    std::array<glm::vec3, 3> verts{a.pos,b.pos,c.pos};

    glm::vec3 minPos = glm::min(glm::min(a.pos,b.pos),c.pos);
    glm::vec3 maxPos = glm::max(glm::max(a.pos,b.pos),c.pos);
    for(int x = minPos.x; x < maxPos.x; x++)
    {
        for(int y = minPos.y ; y < maxPos.y; y++)
        {
            glm::vec3 bc = barycentricCoordinates2D(a.pos, b.pos, c.pos, glm::vec2(x,y)); 
            auto inTriangle = [] (glm::vec3& bc) 
            { 
                return bc.x >= 0.0f && bc.x < 1.0f &&  bc.y >= 0.0f 
                    && bc.y < 1.0f &&  bc.z >= 0.0f && bc.y < 1.0f;
            };
            if(inTriangle(bc))
            {
                T interpolated = (a*bc.x) + (b*bc.y) + (c*bc.z);
                tmp.push_back(interpolated);
            }
        }
    }
    return tmp;
}


#endif
