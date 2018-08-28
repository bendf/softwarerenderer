#define CATCH_CONFIC_ENABLE_ALL_STRINGMAKERS
#include "model.hpp"
#include <catch.hpp>
#include <fstream>
#include <tuple>

TEST_CASE("Quad Model", "[model, io, simple]")
{
    std::ifstream quad("../obj/quad.obj");

    std::vector<MTri> q = LoadModel(quad);
    REQUIRE(q.size() == 2);
    REQUIRE(q[0][0].pos() == glm::vec3(-0.9f, -0.9f, 0.0f));
}
