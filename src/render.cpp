#include "render.hpp"
#include "util.hpp"
glm::vec3 barycentricCoordinates(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p)
{
    // p = up0 + vp1 + wp2
    // w = 1 - u - v
    // magnitude of cross product to calculate parallelogram, half it for triangle size, but we are diving by denom, which is also a triangle, so factor out the divide by 2
    glm::vec3 triNormal = glm::cross(p1 - p0, p2 - p0);
    float denom = glm::length(triNormal);
    glm::vec3 uNorm = glm::cross(p1 - p, p2 - p);
    glm::vec3 vNorm = glm::cross(p2 - p, p0 - p);
    float u =  sgn(glm::dot(uNorm,triNormal)) *  glm::length(uNorm)/denom;
    float v = sgn(glm::dot(vNorm,triNormal)) * glm::length(vNorm)/denom;
    float w = 1.0f - (u + v);

    return glm::vec3(u,v,w);
}


glm::vec3 clipToViewport(glm::vec3 p, int width, int height)
{
    return (p + glm::vec3(1.0f, 1.0f,0.0f)) * glm::vec3(float(width)/2.0f, float(height)/2.0f, 1.0f);
}

