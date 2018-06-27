#include <catch.hpp>
#include "targa.hpp"


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
        REQUIRE_THROWS( t.setPixel(1024, 1024, 1.0f, 0.0f, 0.0f));
        REQUIRE_THROWS( t.setPixel(-1, -1, 0.0f, 1.0f, 1.0f));

        uint8_t r,g,b;
        REQUIRE_THROWS( t.getPixel(-1,-1, r,g,b));
        REQUIRE_THROWS( t.getPixel(1024,1024, r,g,b));

        //Writing
        t.setPixel(0,0, 1.0f, 0.0f,0.0f);
        t.getPixel(0,0,r,g,b); 
        REQUIRE (r == 255);
        REQUIRE (g == 0); 
        REQUIRE (b == 0);
    }

    SECTION( "Targa clearing ")
    {
        Targa clearTest(100,100);
        //Clearing works
        clearTest.clear(1.0f, 1.0f, 1.0f);
        REQUIRE(num_nonzero_pixels(clearTest) == 100*100);
        clearTest.clear(0.0f, 0.0f, 0.0f);
        REQUIRE(num_nonzero_pixels(clearTest) == 0);
    }

    SECTION( "Targa Line Drawing ") 
    {
        Targa lineTest(100, 100);
        SECTION ("point order is irrelevant")
        {
            //Big diagonal line
            lineTest.drawLine(0,0,1023,1023, 1.0f, 0.0f, 0.0f);
            REQUIRE(num_nonzero_pixels(lineTest) == 1024);
            //Drawing backwards still fills same pixels
            lineTest.drawLine(1023, 1023, 0, 0, 1.0f, 1.0f, 1.0f);
            REQUIRE(num_nonzero_pixels(lineTest) == 1024);
        }

        SECTION ("Line may extend beyond bounds")
        {
            lineTest.drawLine(-1,-1, 10,10, 1.0f, 0.0f, 1.0f);
            REQUIRE(num_nonzero_pixels(lineTest) == 10);
        }

        SECTION ("Horizontal lines can be drawn")
        {
            lineTest.drawLine(0, 50, 100, 50, 1.0f, 1.0f, 1.0f);
            REQUIRE(num_nonzero_pixels(lineTest) == 101);
        }

        SECTION ("Vertical lines can be drawn")
        {
            lineTest.drawLine(50, 0, 50, 100, 1.0f, 1.0f, 1.0f);
            REQUIRE(num_nonzero_pixels(lineTest) == 101);
        }

        SECTION("Shallow ascent drawn correctly")
        {
            lineTest.drawLine(0,0, 100, 20, 1.0f, 1.0f, 1.0f);
            REQUIRE(num_nonzero_pixels(lineTest) == 101);
        }

        SECTION("Steep ascent drawn correctly")
        {
            lineTest.drawLine(0,0, 20, 100, 1.0f, 1.0f, 1.0f);
            REQUIRE(num_nonzero_pixels(lineTest) == 101);
        }

        SECTION("Shallow descent drawn correctly")
        {
            lineTest.drawLine(0,100, 100, 80, 1.0f, 1.0f, 1.0f);
            REQUIRE(num_nonzero_pixels(lineTest) == 101);
        }

        SECTION("Steep descent drawn correctly")
        {
            lineTest.drawLine(0,100, 20, 0, 1.0f, 1.0f, 1.0f);
            REQUIRE(num_nonzero_pixels(lineTest) == 101);
        }

    }

    
}
