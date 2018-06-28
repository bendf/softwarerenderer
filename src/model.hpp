#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <tuple>

struct Vertex
{
    int posIndex;
    int uvIndex;
    int normalIndex;
};
bool operator==(const Vertex& lhs, const Vertex& rhs);

struct Triangle {
    struct Vertex a;
    struct Vertex b;
    struct Vertex c;
};
bool operator==(const Triangle& lhs, const Triangle& rhs);

class Model 
{

    private:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        std::vector<Triangle> triangles;
    public:
        size_t getNumVertices();
        size_t getNumNormals();
        size_t getNumUVs();
        size_t getNumTriangles();
        void parseLine(std::string line);
        void LoadAll(const char* filename);
        glm::vec3 vertexAt(int index);
        glm::vec3 normalAt(int index);
        glm::vec2 UVAt(int index);
        Triangle triangleAt(int index);


};
