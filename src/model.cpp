#include "model.hpp"
#include <regex>
#include <sstream>
#include <functional>


std::istream& operator>>(std::istream& stream, glm::vec3& v)
{
	return stream >> v.x >> v.y >> v.z;	
}

std::istream& operator>>(std::istream& stream, const char(&lit))
{
	char read;
	stream >> read;
	if(read != lit)
	{
	    stream.setstate(std::ios::failbit);	
	}
	
	return stream;
}

std::istream& operator>>(std::istream& stream, AttribIndex& ai)
{
	return stream >> ai.pos >> '/' >> ai.uv >> '/' >> ai.norm; 
}

std::vector<MTri> LoadModel(std::istream& stream)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> uvs;
    std::vector<FaceIndex> faceIndices;

    for(std::string s; std::getline(stream,s); !stream.eof())
    {
        if(s.length() > 0) 
        {
            std::stringstream ss(s);
            std::string type;
            ss >> type;
            ss.exceptions(std::ios::failbit); 

            if(type == "f")
            {
                FaceIndex tri;
                ss >> tri[0] >> tri[1] >> tri[2];
                faceIndices.push_back(tri);
            }
            else if(type == "v")
            {
                glm::vec3 v;
                ss >> v;
                positions.push_back(v);
            }
            else if(type == "vt")
            {
                glm::vec3 v;
                ss >> v;
                uvs.push_back(v);
            }
            else if(type == "vn")
            {
                glm::vec3 v;
                ss >> v;
                normals.push_back(v);
            }
        }                   
    }

    auto attribMaker = [&positions, & normals, & uvs](AttribIndex& ai)
    {
        return MVertex{positions[ai.pos-1],normals[ai.norm-1],uvs[ai.uv-1]};
    };

    auto faceMaker = [attribMaker](FaceIndex& fi)
    {
        return MTri{attribMaker(fi[0]), attribMaker(fi[1]), attribMaker(fi[2])};
    };

    std::vector<MTri> temp(faceIndices.size());
    std::transform(faceIndices.begin(), faceIndices.end(), temp.begin(), faceMaker);
}

