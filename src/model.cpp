#include "model.hpp"
#include <sstream>
#include <fstream>
#include <exception>

bool operator==(const Vertex& lhs, const Vertex& rhs)
{
    return (lhs.posIndex == rhs.posIndex) && (lhs.uvIndex == rhs.uvIndex) && (lhs.normalIndex == rhs.normalIndex);
}

bool operator==(const Triangle& lhs, const Triangle& rhs)
{
    return (lhs.a == rhs.a) && (lhs.b == rhs.b) && (lhs.c == rhs.c);
}
size_t Model::getNumVertices()
{
    return vertices.size();
}

size_t Model::getNumNormals()
{
    return normals.size();
}

size_t Model::getNumUVs()
{
    return uvs.size();
}

size_t Model::getNumTriangles()
{
    return triangles.size();
}


glm::vec3 Model::vertexAt(int index)
{
    return vertices.at(index);
}

glm::vec3 Model::normalAt(int index)
{
    return normals.at(index);
}

glm::vec2 Model::UVAt(int index)
{
    return uvs.at(index);
}

Triangle Model::triangleAt(int index)
{
    return triangles.at(index);
}

void Model::LoadAll(const char* filename)
{
    std::ifstream file;
    //throw exceptions
    //file.exceptions (std::fstream::failbit | std::fstream::badbit );
    file.open(filename);
    if(!file.is_open())
    {
        std::stringstream s;
        s << "Failed to open file " << filename;
        throw std::invalid_argument(s.str());
    }
    std::string line;
    while(!file.eof())
    {
        std::getline(file,line);
        parseLine(line);
    }
}

void Model::parseLine(std::string line)
{
    std::stringstream iss(line);
    auto start = line.substr(0,2);    
    std::string trash;
    if(start == "v ")
    {
        float x,y,z; 
        iss >> trash >> x >> y >> z;
        vertices.push_back(glm::vec3(x,y,z));
    }
    else if(start == "vn")
    {
        float x,y,z;
        iss >> trash >> x >> y >> z;
        normals.push_back(glm::vec3(x,y,z));

    }
    else if(start == "vt")
    {
        float u,v;
        iss >> trash >> u >> v;
        uvs.push_back(glm::vec2(u,v));
    }
    else if(start == "f ")
    {
        iss >> trash;
        char ctrash;
        int vi[3],uvi[3],ni[3];
        for(int n =0; n < 3; n++)
        {
            iss >> vi[n] >> ctrash >> uvi[n] >> ctrash >> ni[n];
        }
        Triangle tri = {
            .a = {vi[0]-1, uvi[0]-1, ni[0]-1},
            .b = {vi[1]-1, uvi[1]-1, ni[1]-1},
            .c = {vi[2]-1, uvi[2]-1, ni[2]-1}
        };
        triangles.push_back(tri);

    }

}
