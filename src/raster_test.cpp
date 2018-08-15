#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include <glm/glm.hpp>
#include "raster.hpp"

TEST_CASE("raster2d/triangle", "[raster, triangle]")
{
    
    PosAttr a{glm::vec3(0.0f)};
    PosAttr b{glm::vec3(0.0f, 50.0f, 0.0f)};
    PosAttr c{glm::vec3(50.0f, 0.0f, 0.0f)};

    std::vector<PosAttr> frags = rasterTriangle(a,b,c);
    REQUIRE(frags.size() > 50*24);

}


