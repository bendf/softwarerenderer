#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include "buffer2d.hpp"
#include "targa.hpp"

template<typename T>
unsigned int countBufferValues(Buffer2D<T>& buffer, T value)
{
    unsigned int acc = 0;
    for(int x = 0; x < buffer.getWidth(); x++)
    {
        for(int y = 0; y < buffer.getHeight(); y++)
        {
          acc += (buffer.get(x,y) == value);  
        }
    }
    return acc;
}

TEST_CASE("buffer2d/primitive", "[buffer2d]")
{
    REQUIRE_THROWS(Buffer2D<int>(0,4));
    
    Buffer2D<int> buf(100,100);
    REQUIRE(buf.getWidth() == 100);
    REQUIRE(buf.getHeight() == 100);

    CHECK(buf.isInBounds(10,10));
    CHECK(!buf.isInBounds(-1,-1));
    CHECK(!buf.isInBounds(100,100));
    CHECK_THROWS(buf.get(-1,-1));
    CHECK_THROWS(buf.set(-1,-1,1));

    CHECK_THROWS(buf.get(100,100));
    CHECK_THROWS(buf.set(100,100,1));

    CHECK(buf.getByteSize() == 100*100*sizeof(int));
}

TEST_CASE("buffer2d/line", "[buffer2d line raster]")
{
    Buffer2D<bool> buffer(100,100);
    buffer.clear(false);
    //REQUIRE(countBufferValues(buffer,false) == 100*100);

    SECTION("1:1 diagonal line")
    {
       buffer.drawLine(0,0,99,99,true);  
       REQUIRE(countBufferValues(buffer,true) == 100);
    }

    SECTION("1:1 diagonal line backwards")
    {
        buffer.drawLine(99,99,0,0, true);
        REQUIRE(countBufferValues(buffer,true) == 100);
    }

    SECTION("1:1 diagonal line both ways")
    {
        buffer.drawLine(0,0,99,99,true);  
        buffer.drawLine(99,99,0,0, true);
        REQUIRE(countBufferValues(buffer,true) == 100);
    }

    SECTION("horizontal line")
    {
        buffer.drawLine(0,0, 99,0, true);
        REQUIRE(countBufferValues(buffer,true) == 100);
    }

    SECTION("vertical line")
    {
        buffer.drawLine(0,0,0,99, true);
        REQUIRE(countBufferValues(buffer,true) == 100);
    }

    SECTION("shallow line")
    {
        buffer.drawLine(0,0,99,20, true);
        REQUIRE(countBufferValues(buffer,true) == 100);
    }

    SECTION("steep line")
    {
        buffer.drawLine(0,0,20,99, true);
        REQUIRE(countBufferValues(buffer,true) == 100);
    }

    SECTION("shallow descent")
    {
        buffer.drawLine(0,99,99,80,true);
        REQUIRE(countBufferValues(buffer,true) == 100);
    }
    SECTION("shallow descent")
    {
        buffer.drawLine(0,99,20,0,true);
        REQUIRE(countBufferValues(buffer,true) == 100);
    }

    SECTION("backwards shallow line")
    {
        buffer.drawLine(99,20,0,0, true);
        REQUIRE(countBufferValues(buffer,true) == 100);
    }

    SECTION("backwards steep line")
    {
        buffer.drawLine(20,99,0,0, true);
        REQUIRE(countBufferValues(buffer,true) == 100);
    }

    SECTION("backwards shallow descent")
    {
        buffer.drawLine(99,80,0,99,true);
        REQUIRE(countBufferValues(buffer,true) == 100);
    }
    SECTION("backwards shallow descent")
    {
        buffer.drawLine(20, 0, 0, 99,true);
        REQUIRE(countBufferValues(buffer,true) == 100);
    }

    SECTION("draw line from (x,y) to (x,y)")
    {
        buffer.drawLine(50,50,50,50, true);
        REQUIRE(countBufferValues(buffer,true) == 1);
    }

    SECTION("draw line out of bounds")
    {
        buffer.drawLine(-7,-7, 99, 99, true);
        REQUIRE(countBufferValues(buffer,true) == 100);
        buffer.clear(false);
        buffer.drawLine(0,0, 200,200, true);
        REQUIRE(countBufferValues(buffer,true) == 100);
    }
}


TEST_CASE("buffer2d/triangle", "[buffer2d triangle raster]")
{
    Buffer2D<bool> buffer(200,200);
    buffer.clear(false);
    REQUIRE(countBufferValues(buffer,false) == 200*200);

    SECTION("Half size triangle")
    {
        buffer.drawTriangle(0,0,199,199,199,0,true);
        REQUIRE(countBufferValues(buffer,true) > (199)*(199)/2);
    }

    SECTION("Centered triangle")
    {
        buffer.drawTriangle(50,50,150,150, 175,25, true);
        REQUIRE(countBufferValues(buffer,true) > 7000);
    }

    SECTION("1 Pixel triangle")
    {
        buffer.drawTriangle(100,100,100,100,100,100,true);
        REQUIRE(countBufferValues(buffer,true) == 0);
    }

    SECTION("2 Pixel triangle")
    {
        buffer.drawTriangle(100,100,101,100,101,100,true);
        REQUIRE(countBufferValues(buffer,true) == 0);
    }
    
    SECTION("3 Pixel Triangle")
    {
        buffer.drawTriangle(100,100,101,100,101,101,true);
        REQUIRE(countBufferValues(buffer,true) == 3);
    }

    SECTION("Flat line Triangle")
    {
        buffer.drawTriangle(0,100,100,100,199,100, true);
        REQUIRE(countBufferValues(buffer,true) == 0);
    }

    SECTION("Out of bounds triangle")
    {
        buffer.drawTriangle(-50, -50, 300, 300, 50, -10, true);
        REQUIRE(countBufferValues(buffer,true) > 7000);
    }
}
