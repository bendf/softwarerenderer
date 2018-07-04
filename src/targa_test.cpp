#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include <fstream>
#include "targa.hpp"

TEST_CASE("TARGA", "[targa io]")
{
    Buffer2D<Targa::TargaFormat> imageBuffer(100,100);  
    Targa::TargaFormat white = {255,255,255};
    imageBuffer.clear(white);
    REQUIRE_NOTHROW(Targa::write("targa_blank.tga", imageBuffer));

    std::ifstream f("targa_blank.tga");
    REQUIRE(f.good());

    Buffer2D<Targa::TargaFormat> fromFile = Targa::read("targa_blank.tga");
    REQUIRE(fromFile.getWidth() == 100);
    REQUIRE(fromFile.getHeight() == 100);

    Targa::TargaFormat expected = {255,255,255};
    Targa::TargaFormat found = fromFile.get(0,0);
    CHECK(found.r == expected.r);
    CHECK(found.g == expected.g);
    CHECK(found.b == expected.b);
    

}
