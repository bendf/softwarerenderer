#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include <fstream>
#include "targa.hpp"

TEST_CASE("targa", "[targa io]")
{
    const char* filename = "images/targa_clear_red.tga";
    Buffer2D<Targa::TargaFormat> imageBuffer(100,100);  
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
