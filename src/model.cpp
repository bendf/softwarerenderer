#include "model.hpp"
#include <regex>
#include <sstream>
#include <functional>


bool AttribIndex::operator==(const AttribIndex& o) const
{
    return pos == o.pos && uv == o.uv && norm == o.norm;
}

bool AttribIndex::operator!=(const AttribIndex& o ) const
{
    return !(*this == o);
}

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

void Model::parseLine(const std::string& str)
{
    if(str.length() > 0) 
	{
		std::stringstream ss(str);
		std::string type;
		ss >> type;
		ss.exceptions(std::ios::failbit); 

		if(type == "f")
		{
            std::tuple<AttribIndex,AttribIndex,AttribIndex>  tri;
            ss >> std::get<0>(tri) >> std::get<1>(tri) >> std::get<2>(tri);
            triangles.push_back(tri);  
				
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

Model::Model(std::istream& stream) : positions{}, normals{}, uvs{}, triangles{}
{
    std::stringstream ss;
    std::noskipws(stream);
    std::copy(std::istream_iterator<char>(stream),
              std::istream_iterator<char>(),
              std::ostream_iterator<char>(ss));

    std::string s = ss.str();
    std::regex re("\n");

    try 
    {
        std::for_each(std::sregex_token_iterator(s.begin(), s.end(), re, -1),
                      std::sregex_token_iterator(),
                      std::bind(&Model::parseLine, this, std::placeholders::_1));
    }
    catch(std::ios_base::failure f)
    {
        std::terminate();
    }




         
         
}
