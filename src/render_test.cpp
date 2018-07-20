#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>

#include "render.hpp"
#include "targa.hpp"


namespace Catch {
	template<>
    struct StringMaker<glm::vec3> {
    	static std::string convert( glm::vec3 const& value ) {
            
            std::stringstream s;
            s << "(" << value.x << "," << value.y << "," << value.z << ")";
        	return s.str();
        }
    };
}


TEST_CASE("Render", "[render buffer2d]")
{
        Buffer2D<Targa::TargaFormat> colorBuffer(100,100);
    Buffer2D<float> depthBuffer(100,100);

    REQUIRE(depthBuffer.getHeight() == colorBuffer.getHeight());
    REQUIRE(depthBuffer.getWidth() == colorBuffer.getWidth());

    colorBuffer.clear({0,0,0});
    depthBuffer.clear(-1.0f);

    glm::vec3 a(-1.0f, -1.0f, 0.0f);
    glm::vec3 b(1.0f, -1.0f, 0.0f);
    glm::vec3 c(0, 1.0f, 0.0f);

    SECTION("Draw Triangle no depth")
    {
        renderTriangle(a,b,c, {255,255,255}, &colorBuffer);
        REQUIRE(countBufferValues(colorBuffer, {255,255,255}) > 0);


    }

    SECTION("Non-Matching buffer sizes")
    {
        Buffer2D<bool> largeBuffer(100,100);
        Buffer2D<float> smallDepthBuffer(1,1);
        REQUIRE_THROWS(renderTriangle(glm::vec3(-1,-1,0),glm::vec3(1,-1,0), 
                    glm::vec3(-1,1,0), true, &largeBuffer, &smallDepthBuffer));
    }

}

TEST_CASE("render/barycentric", "[render barycentric triangle]")
{
    glm::vec3 bc = barycentricCoordinates(glm::vec3(1,1,0), glm::vec3(2,2,0), glm::vec3(2,1,0),
            glm::vec3(1,1,0));
    REQUIRE(bc == glm::vec3(1,0,0));

    glm::vec3 a(0,0,0);
    glm::vec3 b(1,0,0);
    glm::vec3 c(0,1,0);

    glm::vec3 centroid = barycentricCoordinates(a,b,c,(a/3.0f)+(b/3.0f)+(c/3.0f));
    REQUIRE(centroid.x == Approx(1.0f/3.0f));
    REQUIRE(centroid.y == Approx(1.0f/3.0f));
    REQUIRE(centroid.z == Approx(1.0f/3.0f));

}

TEST_CASE("render/triangle", "[triangle raster]") {
    Buffer2D<bool> buffer(200,200);
    buffer.clear(false);
    REQUIRE(countBufferValues(buffer,false) == 200*200);

    SECTION("Half size triangle")
    {
        renderTriangle(glm::vec3(0,0,0),glm::vec3(199,199,0),glm::vec3(199,0,0),true, &buffer);
        REQUIRE(countBufferValues(buffer,true) > (199)*(198)/2);
    }

    SECTION("Centered triangle")
    {
        renderTriangle(glm::vec3(50,50,0),glm::vec3(150,150,0),glm::vec3(175,25,0), true, &buffer);
        REQUIRE(countBufferValues(buffer,true) > 7000);
    }

    SECTION("1 Pixel triangle")
    {
        renderTriangle(glm::vec3(100,100,0),glm::vec3(100,100,0),glm::vec3(100,100,0), true, &buffer);
        REQUIRE(countBufferValues(buffer,true) == 0);
    }

    SECTION("2 Pixel triangle")
    {
        renderTriangle(glm::vec3(100,100,0),glm::vec3(101,100,0),glm::vec3(101,100,0), true, &buffer);
        REQUIRE(countBufferValues(buffer,true) == 0);
    }
    
    SECTION("3 Pixel Triangle")
    {
        renderTriangle(glm::vec3(100,100,0),glm::vec3(101,100,0),glm::vec3(101,101,0), true, &buffer);
        REQUIRE(countBufferValues(buffer,true) == 3);
    }

    SECTION("Flat line Triangle")
    {
        renderTriangle(glm::vec3(0,100,0),glm::vec3(100,100,0),glm::vec3(199,100,0), true, &buffer);
        REQUIRE(countBufferValues(buffer,true) == 0);
    }

    SECTION("Out of bounds triangle")
    {
        renderTriangle(glm::vec3(-50,-50,0),glm::vec3(300,300,0),glm::vec3(50,-10,0), true, &buffer);
        REQUIRE(countBufferValues(buffer,true) > 7000);
    }
}
