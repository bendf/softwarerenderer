#include <fstream>
#include <tuple>
#include <vector>
#include <glm/glm.hpp>


struct AttribIndex
{
    size_t pos;
    size_t uv;
    size_t norm;
    bool operator==(const AttribIndex& other) const;
};


class Model
{
    public:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> uvs;

    std::vector<std::tuple<AttribIndex,AttribIndex,AttribIndex>> triangles;
    Model(std::istream& stream);

};

std::istream operator>>(std::istream& stream, glm::vec3& vec);
std::istream operator>>(std::istream& stream, AttribIndex& attribIndex);




