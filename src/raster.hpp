#ifndef RASTER_H
#define RASTER_H
#include<glm/glm.hpp>

glm::vec3 barycentricCoordinatesOrtho(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec2 p);
template<typename OutIter, typename AttrBundle> void rasterLine(
	const AttrBundle &p0,
	const AttrBundle &p1,	
	OutIter iterator);
template<typename OutIter, typename AttrBundle> void rasterTriangle(
	AttrBundle p0,
	AttrBundle p1,
	AttrBundle p2,
	OutIter iterator);



#include "raster_impl.hpp"

#endif
