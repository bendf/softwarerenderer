#include <glm/glm.hpp>
#include <vector>

glm::vec3 barycentricCoordinates2D(const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2, const glm::vec2 p);
std::vector<glm::vec3> rasterTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);

