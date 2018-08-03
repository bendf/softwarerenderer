#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include "raster.hpp"
#include "buffer2d.hpp"
#include "targa.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

std::ostream& operator<<(std::ostream& out, const glm::ivec2& g)
{
        return out << glm::to_string(g);
}

std::ostream& operator<<(std::ostream& out, const glm::vec2& g)
{
        return out << glm::to_string(g);
}

std::ostream& operator<<(std::ostream& out, const glm::vec3& g)
{
        return out << glm::to_string(g);
}



#include <catch.hpp>


struct PosAttr {
	glm::vec2 pos;
	glm::vec2 getPos() const { return pos; }
	PosAttr operator+ (const PosAttr& other) const
	{
		return PosAttr{pos+other.pos};
	}
	PosAttr operator* (float f) const
	{
		return PosAttr{pos*f};
	}
};


TEST_CASE("raster/line", "[raster line iterator]")
{
    std::vector<PosAttr> fragments;
   
    SECTION("1:1 line ") {
		rasterLine(PosAttr{glm::vec2(0)}, PosAttr{glm::vec2(50,50)}, std::back_inserter(fragments));
		REQUIRE(fragments.size() == 51);
		REQUIRE(fragments[10].getPos() == glm::vec2(10,10));
    }

	
    SECTION("1:-1 line (negative slope)")
    {
		rasterLine(PosAttr{glm::vec2(0)}, PosAttr{glm::vec2(50,-50)}, std::back_inserter(fragments));
		REQUIRE(fragments.size() == 51);
		REQUIRE(fragments[10].getPos() == glm::vec2(10,-10));
    }


    SECTION("horizontal line")
    {
		rasterLine(PosAttr{glm::vec2(0)}, PosAttr{glm::vec2(50,0)}, std::back_inserter(fragments));
		REQUIRE(fragments.size() == 51);
		REQUIRE(fragments[10].getPos() == glm::vec2(10,0));
 
    }

    SECTION("vertical line")
    {
		rasterLine(PosAttr{glm::vec2(0)}, PosAttr{glm::vec2(0,50)}, std::back_inserter(fragments));
		REQUIRE(fragments.size() == 51);
		REQUIRE(fragments[10].getPos() == glm::vec2(0,10));
    }


    SECTION("steep negative line")
    {
		rasterLine(PosAttr{glm::vec2(0)}, PosAttr{glm::vec2(20,-50)}, std::back_inserter(fragments));
		REQUIRE(fragments.size() == 51);
		REQUIRE(fragments[10].getPos() == glm::vec2(16,-40));
 
    }

    SECTION("shallow backwards line")
    {
		rasterLine(PosAttr{glm::vec2(-50,20)}, PosAttr{glm::vec2(0)}, std::back_inserter(fragments));
		REQUIRE(fragments.size() == 51);
		REQUIRE(fragments[10].getPos() == glm::vec2(-40,16));
 
    }


	/*
    SECTION("Visual inspection")
    {
        Buffer2D<Targa::TargaFormat> canvas(100,100);
        canvas.clear({0,0,0});
        rasterLine(glm::vec3(50,20,0), glm::vec3(0,0,0), std::back_inserter(interpolators));
        rasterLine(glm::vec3(10,10,0), glm::vec3(70,70,0), std::back_inserter(interpolators));
        rasterLine(glm::vec3(99,99,0), glm::vec3(80,90,0), std::back_inserter(interpolators));
        rasterLine(glm::vec3(0,99,0), glm::vec3(20,0,0), std::back_inserter(interpolators));
        rasterLine(glm::vec3(0,99,0), glm::vec3(50,80,0), std::back_inserter(interpolators));
        
		
        for(auto p : interpolators)
        {
		   glm::vec3 color = glm::vec3((p.second.x * blue) + (p.second.y * red));
           canvas.set(p.first.x, p.first.y, Targa::fromVec3(color)); 
        }
        
        Targa::write("images/raster_visual_test_line.tga", canvas);

    }
	*/
}

/*

TEST_CASE("raster triangle", "[raster triangle iterator]")
{
    std::vector<glm::vec3> interpolators;  

    SECTION("Right angled triangle")
    {
        rasterTriangle(glm::vec3(0,0,0), glm::vec3(0,10,0), glm::vec3(10,0,0), std::back_inserter(interpolators));
        REQUIRE(interpolators.size() == 50 + 11);
        REQUIRE(interpolators[0] == std::make_pair(glm::ivec2(0,0), glm::vec3(0.0f)));

    }


    SECTION("skew triangle")
    {
        rasterTriangle(glm::vec3(10,10,0), glm::vec3(5,80,0), glm::vec3(80,50,0), std::back_inserter(interpolators));
        REQUIRE(interpolators.size() > 100);
        REQUIRE(interpolators[0] == std::make_pair(glm::ivec2(6,66), glm::vec3(0.2f, 0.8f, 0.0f)));

    }


    SECTION("Visual inspection")
    {
        Buffer2D<Targa::TargaFormat> canvas(1000,1000);
        canvas.clear({0,0,0});

        rasterTriangle(glm::vec3(100,100,0), glm::vec3(50,800,0), glm::vec3(800,500,0), std::back_inserter(interpolators));
        
		
		glm::vec3 red(1.0f,0.0f,0.0f);
		glm::vec3 blue(0.0f,1.0f,0.0f);
		glm::vec3 green(0.0f,0.0f,1.0f);
        for(auto p : interpolators)
        {
		   glm::vec3 color = glm::mat3x3(red,blue,green) * p.second;
           canvas.set(p.first.x, p.first.y, Targa::fromVec3(color)); 
        }
        
        Targa::write("images/raster_visual_test_triangle.tga", canvas);

    }




}
*/
