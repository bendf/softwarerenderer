#include <fstream>
#include <tuple>
#include <vector>
#include <glm/glm.hpp>
#include "attributes.hpp"


struct AttribIndex
{
    size_t pos;
    size_t uv;
    size_t norm;
    bool operator==(const AttribIndex& other) const;
    bool operator!=(const AttribIndex& other) const;
};


class Model
{
    using MAttrs = Attributes<glm::vec3,glm::vec3,glm::vec3>;
    using MTri = std::tuple<MAttrs,MAttrs, MAttrs>;

    private:
    void parseLine(const std::string& str);
    public:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> uvs;

    std::vector<std::tuple<AttribIndex,AttribIndex,AttribIndex>> triangles;
    Model(std::istream& stream);
    auto begin() {return iterator(this,triangles.begin());}
    auto end() {return iterator(this,triangles.end());}
    class iterator 
    {
        public:
        decltype(triangles)::iterator it; 
        Model* ptr;
        iterator( Model* ptr, decltype(triangles)::iterator it) : it(it), ptr(ptr) {}
        MTri operator*() 
        {
            std::tuple<AttribIndex,AttribIndex,AttribIndex>& ai = *it;
            MAttrs a{
                ptr->positions[std::get<0>(ai).pos-1],
                ptr->normals[std::get<0>(ai).norm-1],
                ptr->uvs[std::get<0>(ai).uv-1]
            };
            MAttrs b{
                ptr->positions[std::get<1>(ai).pos-1],
                ptr->normals[std::get<1>(ai).norm-1],
                ptr->uvs[std::get<1>(ai).uv-1]
            };
            MAttrs c{
                ptr->positions[std::get<2>(ai).pos-1],
                ptr->normals[std::get<2>(ai).norm-1],
                ptr->uvs[std::get<2>(ai).uv-1]
            };

            return std::make_tuple(a,b,c); 
        }
        iterator operator++() { it++; }
        bool operator==(const iterator& other ) const 
        {
            return (ptr == other.ptr) && (it == other.it);
        }

        bool operator!=(const iterator& other) const
        {
            return !(*this == other);
        }

    };

};

std::istream& operator>>(std::istream& stream, glm::vec3& vec);
std::istream& operator>>(std::istream& stream, AttribIndex& attribIndex);




