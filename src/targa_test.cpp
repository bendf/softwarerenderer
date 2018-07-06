#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include <fstream>
#include "targa.hpp"

TEST_CASE("targa", "[targa io]")
{
    Buffer2D<Targa::TargaFormat> imageBuffer(100,100);  

    SECTION("Clear Red targa")
    {
        const char* filename = "images/targa_clear_red.tga";
        Targa::TargaFormat red = {0,0,255};
        imageBuffer.clear(red);
        REQUIRE_NOTHROW(Targa::write(filename, imageBuffer));

        std::ifstream f(filename);
        REQUIRE(f.good());

        Buffer2D<Targa::TargaFormat> fromFile = Targa::read(filename);
        REQUIRE(fromFile.getWidth() == 100);
        REQUIRE(fromFile.getHeight() == 100);

        
        Targa::TargaFormat expected = {0,0,255};
        Targa::TargaFormat found = fromFile.get(0,0);
        CHECK(found.r == expected.r);
        CHECK(found.g == expected.g);
        CHECK(found.b == expected.b);
    }

    SECTION("Blue lines - visual inspection")
    {
        const char* filename = "images/blue_lines.tga";
        Targa::TargaFormat blue = {255,0,0};
        imageBuffer.drawLine(-7,-7, 20, 20,blue);
        imageBuffer.drawLine(0,99, 20, 0,blue);
        imageBuffer.drawLine(0,0,30,99, blue);
        imageBuffer.drawLine(0,50,99,50, blue);
        imageBuffer.drawLine(0,0,0,100,blue);
        imageBuffer.drawLine(0,99,99,80,blue);
        Targa::write(filename, imageBuffer);

    }

    SECTION("Yellow triangles - visual inspection")
    {
        Buffer2D<Targa::TargaFormat> tri_buffer(400,400);
        tri_buffer.clear({0,0,0});
        const char* filename = "images/yellow_triangles.tga";
        Targa::TargaFormat yellow = {0, 255,255};
        tri_buffer.drawTriangle(0,0, 400,400,400,0, yellow);
        tri_buffer.drawTriangle(0,400, 100,390,50,150, yellow);
        Targa::write(filename, tri_buffer);

    }

}



