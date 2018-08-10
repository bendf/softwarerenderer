#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include <glm/glm.hpp>
#include "raster.hpp"

TEST_CASE("raster2d/triangle", "[raster, triangle]")
{
    
    glm::vec3 a(0.0f);
    glm::vec3 b(0.0f, 50.0f, 0.0f);
    glm::vec3 c(50.0f, 0.0f, 0.0f);

    std::vector<glm::vec3> bcs = rasterTriangle(a,b,c);
    REQUIRE(bcs.size() > 50*24);

}


