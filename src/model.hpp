#include "attributes.hpp"
#include <fstream>
#include <glm/glm.hpp>
#include <tuple>
#include <vector>

using MVertex = Attributes<glm::vec3, glm::vec3, glm::vec3>;
using MTri = std::array<MVertex, 3>;

struct AttribIndex {
    size_t pos, uv, norm;
};
using FaceIndex = std::array<AttribIndex, 3>;

std::vector<MTri> LoadModel(std::istream& stream);

std::istream& operator>>(std::istream& stream, glm::vec3& vec);
std::istream& operator>>(std::istream& stream, AttribIndex& attribIndex);
std::istream& operator>>(std::istream& stream, const char(&lit));
