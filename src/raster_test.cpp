#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include "raster.hpp"
#include <catch.hpp>
#include <glm/glm.hpp>

TEST_CASE("raster2d/triangle", "[raster, triangle]")
{
    Attributes<glm::vec3> a{ glm::vec3(-1.0f, -1.0f, 0.0f) };
    Attributes<glm::vec3> b{ glm::vec3(-1.0f, 0.0f, -1.0f) };
    Attributes<glm::vec3> c{ glm::vec3(0.0f, -1.0f, -1.0f) };

    std::vector<std::pair<glm::ivec2,Attributes<glm::vec3>>> frags = rasterTriangle(a, b, c, 100, 100);
    for(auto f : frags)
    {
        REQUIRE(f.first.x < 50);
        REQUIRE(f.first.y< 50);
    }
    REQUIRE(frags.size() > 50 * 24);
}
