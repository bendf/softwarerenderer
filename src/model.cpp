#include "model.hpp"
#include <sstream>
#include <fstream>
#include <exception>

namespace Model {
    std::vector<AttribBundle> LoadAll(const char* filename)
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
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<AttribBundle> attribBundles;
        while(!file.eof())
        {
            std::getline(file,line);
            parseLine(line, vertices, uvs, normals, attribBundles);
        }
        return attribBundles;

    }

    void parseLine(std::string line,
            std::vector<glm::vec3>& vertices,
            std::vector<glm::vec2>& uvs,
            std::vector<glm::vec3>& normals,
            std::vector<AttribBundle>& attribBundles)
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
            int vi,uvi,ni;
            for(int n =0; n < 3; n++)
            {
                iss >> vi >> ctrash >> uvi >> ctrash >> ni;
                attribBundles.push_back({
                    vertices[vi-1], uvs[uvi-1], normals[ni-1]         
                });
            }
        }

    }

}
