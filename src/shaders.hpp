#include "attributes.hpp"


template<typename ...Is, typename ...Os>
using VertexShader = std::function<AttrTuple<Is...>,(AttrTuple<Os...>);
using FragmentShader = std::function<>;
