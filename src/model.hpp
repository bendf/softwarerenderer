#include <glm/glm.hpp>

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
        int getNumNormals();
        int getNumVertices();
        int getNumUVs();
        int getNumTriangles();
        void parseLine(std::string line);


};
