#include <glm/glm.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <string>
namespace Model {

    struct AttribBundle {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 normal;
    };

    void parseLine(std::string line,
            std::vector<glm::vec3>& vertices,
            std::vector<glm::vec2>& uvs,
            std::vector<glm::vec3>& normals,
            std::vector<AttribBundle>& attribBundles);
    std::vector<AttribBundle> LoadAll(const char* filename);

}
