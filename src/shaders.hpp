#include "attributes.hpp"


template<typename ...Is, typename ...Os>
using VertexShader = std::function<Attributes<Is...>,(Attributes<Os...>);
using FragmentShader = std::function<>;
