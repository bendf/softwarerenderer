#ifndef RENDER_H
#define RENDER_H
#include <glm/glm.hpp>
#include "buffer2d.hpp"


template<typename T>
void renderLine(glm::vec3 p0, glm::vec3 p1, glm::vec3 color0, glm::vec3 color1, Buffer2D<T>* buffer);

template<typename T>
void renderTriangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec2 vt0, glm::vec2 vt1, glm::vec2 vt2, Buffer2D<T>* buffer, Buffer2D<float>* depthBuffer = nullptr, Buffer2D<float>* textureBuffer = nullptr);

glm::vec3 barycentricCoordinates(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p);
glm::vec3 clipToViewport(glm::vec3 p, int width, int height); 
#include "render_impl.hpp"
#endif

