#include <catch.hpp>
#include "targa.hpp"
#include <fstream>
#include <glm/glm.hpp>


unsigned int num_nonzero_pixels(Targa& t) 
{
   unsigned int acc = 0; 
   for( int x = 0; x < t.getWidth(); x++)
   {
       for(int y =0; y < t.getHeight(); y++)
       {
           uint8_t r,g,b;
           t.getPixel(x,y,r,g,b);
           if(r >0 || g > 0 || b > 0)
           {
               acc++;
           }
       }
   }
   return acc;
}

TEST_CASE("TARGA", "[targa io]") 
{
   
    SECTION(" Targa Pixel setting") 
    {
        int width = 1024, height = 1024;
        Targa t(width, height);
        REQUIRE(t.getWidth() == width);
        REQUIRE(t.getHeight() == height);
 
        //Out of bounds
        REQUIRE_THROWS( t.setPixel(1024, 1024, Targa::white));
        REQUIRE_THROWS( t.setPixel(-1, -1, Targa::white));

        uint8_t r,g,b;
        REQUIRE_THROWS( t.getPixel(-1,-1, r,g,b));
        REQUIRE_THROWS( t.getPixel(1024,1024, r,g,b));

        //Writing
        t.setPixel(0,0, Targa::red);
        t.getPixel(0,0,r,g,b); 
        REQUIRE (r == 255);
        REQUIRE (g == 0); 
        REQUIRE (b == 0);
    }

    SECTION( "Targa writing")
    {
        Targa writeTest(100, 100);
        writeTest.clear(Targa::white);
        writeTest.drawLine(0,0, 100, 100, Targa::white);
        writeTest.drawLine(0,100, 20, 80, Targa::white);
        writeTest.drawLine(50,0, 50, 100, Targa::white);
        std::fstream fileOut;
        fileOut.open("targa_test.tga");
        REQUIRE( fileOut.is_open());
        writeTest.write(fileOut);
        fileOut.close();
        REQUIRE(!fileOut.is_open());
    }

    SECTION( "Targa clearing ")
    {
        Targa clearTest(100,100);
        //Clearing works
        clearTest.clear(Targa::white);
        REQUIRE(num_nonzero_pixels(clearTest) == 100*100);
        clearTest.clear(Targa::black);
        REQUIRE(num_nonzero_pixels(clearTest) == 0);
    }

    SECTION( "Targa Line Drawing ") 
    {
        Targa lineTest(100, 100);
        SECTION ("point order is irrelevant")
        {
            //Big diagonal line
            lineTest.drawLine(0,0,99,99, Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
            //Drawing backwards still fills same pixels
            lineTest.drawLine(99,99, 0, 0, Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION ("Line may extend beyond bounds")
        {
            lineTest.drawLine(-1,-1, 10,10, Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 11);
        }

        SECTION ("Horizontal lines can be drawn")
        {
            lineTest.drawLine(0, 50, 99, 50, Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION ("Vertical lines can be drawn")
        {
            lineTest.drawLine(50, 0, 50, 99, Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Shallow ascent drawn correctly")
        {
            lineTest.drawLine(0,0, 99, 20, Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Steep ascent drawn correctly")
        {
            lineTest.drawLine(0,0, 20, 99, Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Shallow descent drawn correctly")
        {
            lineTest.drawLine(0,99, 99, 80, Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Steep descent drawn correctly")
        {
            lineTest.drawLine(0,99, 20, 0, Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Shallow backwards ascent drawn correctly")
        {
            lineTest.drawLine(99, 20, 0, 0, Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Steep backwards ascent drawn correctly")
        {
            lineTest.drawLine(20, 99, 0, 0, Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Shallow backawards descent drawn correctly")
        {
            lineTest.drawLine(99, 80, 0, 99, Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Steep descent drawn correctly")
        {
            lineTest.drawLine(20, 0, 0, 99, Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }
    }
    
}


TEST_CASE("OBJ Loading", "[obj io vertex]")
{

}
