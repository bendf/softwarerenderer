#include<glm/glm.hpp>

template<typename O> void rasterLine(unsigned int vpWidth, unsigned int vpHeight, glm::vec3 p0, glm::vec3 p1, O iterator);
template<typename O> void rasterTriangle(unsigned int vpWidth, unsigned int vpHeight, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, O iterator);



#include "raster_impl.hpp"

