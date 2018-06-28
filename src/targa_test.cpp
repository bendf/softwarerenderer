#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include "targa.hpp"
#include "model.hpp"
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
    SECTION("OBJ line Parsing")
    {
        Model m;

        REQUIRE(m.getNumVertices() == 0);
        m.parseLine("v 1.0 1.0 1.0"); 
        REQUIRE(m.getNumVertices() == 1);
        REQUIRE(m.vertexAt(0) == glm::vec3(1.0f,1.0f,1.0f));

        REQUIRE(m.getNumUVs() == 0);
        m.parseLine("vt 1.0 1.0 "); 
        REQUIRE(m.getNumUVs() == 1);
        REQUIRE(m.UVAt(0) == glm::vec2(1.0f,1.0f));

        REQUIRE(m.getNumNormals() == 0);
        m.parseLine("vn 1.0 1.0  0.5"); 
        REQUIRE(m.getNumNormals() == 1);
        REQUIRE(m.normalAt(0) == glm::vec3(1.0f,1.0f,0.5f));

        REQUIRE(m.getNumTriangles() == 0);
        m.parseLine("f 1/1/1 2/2/2 3/3/3"); 
        REQUIRE(m.getNumTriangles() == 1);
        struct Vertex v0 = {0,0,0};
        struct Vertex v1 = {1,1,1};
        struct Vertex v2 = {2,2,2};
        struct Triangle  tri= {.a = v0, .b = v1, .c = v2};
        REQUIRE(m.triangleAt(0).a == tri.a);
        REQUIRE(m.triangleAt(0).b == tri.b);
        REQUIRE(m.triangleAt(0).c == tri.c);
    }


    SECTION(" OBJ file parsing")
    {
            Model m;
            REQUIRE_THROWS(m.LoadAll("fake_madeup_filename.ducks"));
            m.LoadAll("../obj/african_head.obj");
            REQUIRE(m.getNumVertices() == 1258);
            REQUIRE(m.getNumNormals() == 1258);
            REQUIRE(m.getNumUVs() == 1339);
            REQUIRE(m.getNumTriangles() == 2492);
    }


    SECTION(" Draw obj file") 
    {
        Targa t(1000,1000);
        Model m;
        m.LoadAll("../obj/african_head.obj");

        for(int i = 0; i < m.getNumTriangles(); i++)
        {
            Triangle tri = m.triangleAt(i);
            glm::vec3 verts[3];
            verts[0] = m.vertexAt(tri.a.posIndex);
            verts[1] = m.vertexAt(tri.b.posIndex);
            verts[2] = m.vertexAt(tri.c.posIndex);
            for(int n =0; n <3; n++)
            {
                int x0 = (verts[(n+0)%3].x + 1.0f)* (t.getWidth() /2.0f);
                int x1 = (verts[(n+1)%3].x + 1.0f)* (t.getWidth() /2.0f);
                int y0 = (verts[(n+0)%3].y + 1.0f)* (t.getHeight() /2.0f);
                int y1 = (verts[(n+1)%3].y + 1.0f)* (t.getHeight() /2.0f);
                t.drawLine(x0,y0,x1,y1, Targa::red);
            }
            
        }
        std::ofstream headOut;
        headOut.open("head_wireframe.tga");
        REQUIRE(headOut.is_open());
        t.write(headOut);
        

    }
}
