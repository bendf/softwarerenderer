#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include "buffer2d.hpp"


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


