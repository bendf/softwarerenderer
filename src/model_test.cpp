#define CATCH_CONFIC_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include <fstream> 
#include <tuple>
#include "model.hpp"



TEST_CASE("Quad Model", "[model, io, simple]")
{
    std::ifstream quad("../obj/quad.obj");
    
    Model q(quad);
    REQUIRE(q.positions.at(0) == glm::vec3(-1.0f, -1.0f, 0.0f));
    REQUIRE(q.positions.at(3) == glm::vec3(1.0f,1.0f,0.0f));
    REQUIRE(q.uvs.at(0) == glm::vec3(0.0f, 0.0f, 0.0f));
    REQUIRE(q.uvs.at(3) == glm::vec3(1.0f,1.0f,0.0f));
    REQUIRE(q.normals.at(0) == glm::vec3(0.0f,0.0f, -1.0f));
    

    std::tuple<AttribIndex,AttribIndex,AttribIndex>& t0 = q.triangles[0];
    REQUIRE(std::get<0>(t0) == AttribIndex{1,1,1});
    REQUIRE(std::get<1>(t0) == AttribIndex{2,2,1});
    REQUIRE(std::get<2>(t0) == AttribIndex{3,3,1});
}

TEST_CASE("Model", "[model io]")
{
    std::ifstream obj("../obj/african_head.obj");
    Model head(obj); 

    //for(auto p : Model)
    {

    }
}
