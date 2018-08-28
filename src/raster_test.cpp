#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include "raster.hpp"
#include <catch.hpp>
#include <glm/glm.hpp>

TEST_CASE("raster2d/triangle", "[raster, triangle]")
{

    Attributes<glm::vec3> a{ glm::vec3(0.0f) };
    Attributes<glm::vec3> b{ glm::vec3(0.0f, 50.0f, 0.0f) };
    Attributes<glm::vec3> c{ glm::vec3(50.0f, 0.0f, 0.0f) };

    std::vector<Attributes<glm::vec3>> frags = rasterTriangle(a, b, c);
    REQUIRE(frags.size() > 50 * 24);
}
