#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <tuple>
typedef std::tuple<int,int,int> attribute;
typedef std::tuple<attribute,attribute,attribute> triangle ;
class Model 
{

    private:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        std::vector<triangle> triangles;
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
        triangle triangleAt(int index);


};
