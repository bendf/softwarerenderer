#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include "raster.hpp"
#include "buffer2d.hpp"
#include "targa.hpp"


TEST_CASE("raster/line", "[raster line iterator]")
{
    std::vector<std::pair<int,int>> pixels;
   
    SECTION("1:1 line ") {
        rasterLine(glm::vec3(0,0,0), glm::vec3(50,50,0), std::back_inserter(pixels));
        REQUIRE(pixels.size() == 51);
        REQUIRE(pixels[10] == std::make_pair(10,10));
    }

    SECTION("1:-1 line (negative slope)")
    {
        rasterLine(glm::vec3(0,0,0), glm::vec3(50,-50,0), std::back_inserter(pixels));
        REQUIRE(pixels.size() == 51);
        REQUIRE(pixels[10] == std::make_pair(10,-10));
    }


    SECTION("horizontal line")
    {
        rasterLine(glm::vec3(0,0,0), glm::vec3(50,0,0), std::back_inserter(pixels));
        REQUIRE(pixels.size() == 51);
        REQUIRE(pixels[10] == std::make_pair(10,0));
    }

    SECTION("vertical line")
    {
        rasterLine(glm::vec3(0,0,0), glm::vec3(0,50,0), std::back_inserter(pixels));
        REQUIRE(pixels.size() == 51);
        REQUIRE(pixels[10] == std::make_pair(0,10));
    }

    SECTION("steep negative line")
    {
        rasterLine(glm::vec3(0,0,0), glm::vec3(20,-50,0), std::back_inserter(pixels));
        REQUIRE(pixels.size() == 51);
        REQUIRE(pixels[10] == std::make_pair(16,-40));
    }

    SECTION("shallow backwards line")
    {
        rasterLine(glm::vec3(-50,20,0), glm::vec3(0,0,0), std::back_inserter(pixels));
        REQUIRE(pixels.size() == 51);
        REQUIRE(pixels[10] == std::make_pair(-40,16));
    }


    SECTION("Visual inspection")
    {
        Buffer2D<Targa::TargaFormat> canvas(100,100);
        canvas.clear({0,0,0});
        rasterLine(glm::vec3(50,20,0), glm::vec3(0,0,0), std::back_inserter(pixels));
        rasterLine(glm::vec3(10,10,0), glm::vec3(70,70,0), std::back_inserter(pixels));
        rasterLine(glm::vec3(99,99,0), glm::vec3(80,90,0), std::back_inserter(pixels));
        rasterLine(glm::vec3(0,99,0), glm::vec3(20,0,0), std::back_inserter(pixels));
        rasterLine(glm::vec3(0,99,0), glm::vec3(50,80,0), std::back_inserter(pixels));
        
        Targa::TargaFormat blue = {255,0,0};
        for(auto p : pixels)
        {
           canvas.set(p.first, p.second, blue); 
        }
        
        Targa::write("images/raster_visual_test.tga", canvas);

    }
}
