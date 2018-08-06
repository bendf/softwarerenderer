#include "model.hpp"


bool AttribIndex::operator==(const AttribIndex& o) const
{
    return pos == o.pos && uv == o.uv && norm == o.norm;
}


Model::Model(std::istream& stream)
{


}
