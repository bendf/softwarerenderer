#define CATCH_CONFIC_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include <fstream> 
#include <tuple>
#include "model.hpp"



TEST_CASE("Quad Model", "[model, io, simple]")
{
    std::ifstream quad("../obj/quad.obj");
    
    std::vector<MTri> q = LoadModel(quad);
    REQUIRE(q.size() == 2);
}

