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
           TargaFormat color = t.getPixel(glm::ivec2(x,y));
           if(color.r >0 || color.g > 0 || color.b > 0)
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
        REQUIRE_NOTHROW( t.setPixel(glm::ivec2(1024, 1024), Targa::white));
        REQUIRE_NOTHROW( t.setPixel(glm::ivec2(-1, -1), Targa::white));

        REQUIRE_THROWS( t.getPixel(glm::ivec2(-1,-1)));
        REQUIRE_THROWS( t.getPixel(glm::ivec2(1024,1024)));

        //Writing
        t.setPixel(glm::ivec2(0,0), Targa::red);
        TargaFormat color = t.getPixel(glm::ivec2(0,0)); 
        REQUIRE (color.r == 255);
        REQUIRE (color.g == 0); 
        REQUIRE (color.b == 0);
    }

    SECTION( "Targa writing")
    {
        Targa writeTest(100, 100);
        writeTest.clear(Targa::white);
        writeTest.drawLine(glm::ivec2(0,0), glm::ivec2(100, 100), Targa::white);
        writeTest.drawLine(glm::ivec2(0,100), glm::ivec2(20, 80), Targa::white);
        writeTest.drawLine(glm::ivec2(50,0), glm::ivec2(50, 100), Targa::white);
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
        lineTest.clear(Targa::black);
        SECTION ("point order is irrelevant")
        {
            //Big diagonal line
            lineTest.drawLine(glm::ivec2(0,0),glm::ivec2(99,99), Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
            //Drawing backwards still fills same pixels
            lineTest.drawLine(glm::ivec2(99,99), glm::ivec2(0, 0), Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION ("Line may extend beyond bounds")
        {
            lineTest.drawLine(glm::ivec2(-1,-1), glm::ivec2(10,10), Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 11);
        }

        SECTION ("Horizontal lines can be drawn")
        {
            lineTest.drawLine(glm::ivec2(0, 50), glm::ivec2(99, 50), Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION ("Vertical lines can be drawn")
        {
            lineTest.drawLine(glm::ivec2(50, 0), glm::ivec2(50, 99), Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Shallow ascent drawn correctly")
        {
            lineTest.drawLine(glm::ivec2(0,0), glm::ivec2(99, 20), Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Steep ascent drawn correctly")
        {
            lineTest.drawLine(glm::ivec2(0,0), glm::ivec2(20, 99), Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Shallow descent drawn correctly")
        {
            lineTest.drawLine(glm::ivec2(0,99), glm::ivec2(99, 80), Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Steep descent drawn correctly")
        {
            lineTest.drawLine(glm::ivec2(0,99), glm::ivec2(20, 0), Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Shallow backwards ascent drawn correctly")
        {
            lineTest.drawLine(glm::ivec2(99, 20), glm::ivec2(0, 0), Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Steep backwards ascent drawn correctly")
        {
            lineTest.drawLine(glm::ivec2(20, 99), glm::ivec2(0, 0), Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Shallow backawards descent drawn correctly")
        {
            lineTest.drawLine(glm::ivec2(99, 80), glm::ivec2(0, 99), Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }

        SECTION("Steep descent drawn correctly")
        {
            lineTest.drawLine(glm::ivec2(20, 0), glm::ivec2(0, 99), Targa::white);
            REQUIRE(num_nonzero_pixels(lineTest) == 100);
        }
        SECTION("Line from (x,y) to (x,y) -- aka same location")
        {
            lineTest.drawLine(glm::ivec2(50,50),glm::ivec2(50,50), Targa::red);
            REQUIRE(num_nonzero_pixels(lineTest) == 1);
            
        }
    }

    SECTION( "Targa Triangle Drawing")
    {
        
        Targa triangleTest(100,100);
        glm::ivec2 A(0,0);
        glm::ivec2 B(50,50);
        glm::ivec2 C(99,20);
        //Forwards
        triangleTest.drawTriangle(A, B, C, Targa::red);

        CHECK( num_nonzero_pixels(triangleTest) == 2002);
        //Order of points has no effect
        triangleTest.drawTriangle(B, A, C, Targa::red);
        CHECK(num_nonzero_pixels(triangleTest) == 2002);
        //Order of points has no effect
        triangleTest.drawTriangle(C, B, A, Targa::red);
        CHECK(num_nonzero_pixels(triangleTest) == 2002);

        //Draw triangle extending beyond bounds
        triangleTest.drawTriangle(glm::ivec2(-20,-20), glm::ivec2(50, 80), glm::ivec2(150, 5) ,Targa::white);
        triangleTest.drawTriangle(glm::ivec2(10,10), glm::ivec2(10, 80), glm::ivec2(80,10), Targa::red);

        std::ofstream triangleOut;
        triangleOut.open("triangle.tga");
        REQUIRE( triangleOut.is_open());
        triangleTest.write(triangleOut);
        
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


    SECTION(" Draw obj file wireframe") 
    {
        Targa t(1000,1000);
        t.clear(Targa::green);
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
                PixelCoord p0 = t.fromClip(verts[(n+0)%3]);
                PixelCoord p1 = t.fromClip(verts[(n+1)%3]);
                t.drawLine(p0,p1, Targa::red);
            }
            
        }
        std::ofstream headOut;
        headOut.open("head_wireframe.tga");
        REQUIRE(headOut.is_open());
        t.write(headOut);
    }

    SECTION(" Draw obj file triangles") 
    {
        Targa t(1024,1024);
        t.clear(Targa::black);
        Model m;
        m.LoadAll("../obj/african_head.obj");

        for(int i = 0; i < m.getNumTriangles(); i++)
        {
            Triangle tri = m.triangleAt(i);
            glm::vec3 verts[3];
            verts[0] = m.vertexAt(tri.a.posIndex);
            verts[1] = m.vertexAt(tri.b.posIndex);
            verts[2] = m.vertexAt(tri.c.posIndex);
            glm::vec3 faceNormal = glm::cross(verts[1] - verts[0], verts[2] - verts[0]);
            if(faceNormal.z > 0)
            {
                PixelCoord p0 = t.fromClip(verts[0]);
                PixelCoord p1 = t.fromClip(verts[1]);
                PixelCoord p2 = t.fromClip(verts[2]);

                glm::vec3 lightDir(0,0,1);
                float intensity = glm::dot(lightDir,glm::normalize(faceNormal));
                t.drawTriangle(p0,p1,p2, glm::vec3(1.0f,1.0f,1.0f)*intensity);
            }
            //}
            
        }
        std::ofstream headOut;
        headOut.open("head_triangle.tga");
        REQUIRE(headOut.is_open());
        t.write(headOut);
    }

}
