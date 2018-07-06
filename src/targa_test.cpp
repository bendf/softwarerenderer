#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include <fstream>
#include "targa.hpp"
#include "model.hpp"

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

    SECTION("Green Head wireframe - visual inspection")
    {
        Buffer2D<Targa::TargaFormat> head_buffer(1000,1000);
        head_buffer.clear({0,0,0});
        const char* filename = "images/head_wireframe.tga";
        Model m;
        m.LoadAll("../obj/african_head.obj");
        for(int i = 0; i < m.getNumTriangles(); i++)
        {
            Triangle t = m.triangleAt(i);
            glm::vec3 verts[3]; 
            verts[0] = m.vertexAt(t.a.posIndex);
            verts[1] = m.vertexAt(t.b.posIndex);
            verts[2] = m.vertexAt(t.c.posIndex);

            int width = head_buffer.getWidth();
            int height = head_buffer.getHeight();
            for(int i = 0; i < 3; i++)
            {
                glm::vec2 p0 = clipToViewport(verts[(i+0)%3], width, height);
                glm::vec2 p1 = clipToViewport(verts[(i+1)%3], width, height);

                head_buffer.drawLine(p0.x,p0.y,p1.x,p1.y,{0,255,0});
            }
        }
        Targa::write(filename, head_buffer);
    }

    SECTION(" Triangle Head wireframe - visual inspection")
    {
        Buffer2D<Targa::TargaFormat> head_buffer(1000,1000);
        Buffer2D<float> z_buffer(1000,1000);
        z_buffer.clear(-1.0f);
        head_buffer.clear({0,0,0});
        const char* filename = "images/head_triangle_lit.tga";
        Model m;
        m.LoadAll("../obj/african_head.obj");
        for(int i = 0; i < m.getNumTriangles(); i++)
        {
            Triangle t = m.triangleAt(i);
            glm::vec3 verts[3]; 
            verts[0] = m.vertexAt(t.a.posIndex);
            verts[1] = m.vertexAt(t.b.posIndex);
            verts[2] = m.vertexAt(t.c.posIndex);
            glm::vec3 normals[3];
            normals[0] = m.normalAt(t.a.normalIndex);
            normals[1] = m.normalAt(t.b.normalIndex);
            normals[2] = m.normalAt(t.c.normalIndex);
            

            glm::vec3 faceNormal = glm::normalize(glm::cross(verts[1] - verts[0], verts[2] - verts[0]));
            if(faceNormal.z <= 0)
            {
                continue;
            }
            glm::vec3 diffuse(0.7f, 0.5f, 0.4f);
            glm::vec3 lightDir = glm::normalize(glm::vec3(0.3,0.5,1));
            float intensities[3];
            intensities[0] = glm::clamp(glm::dot(normals[0],lightDir),0.0f,1.0f);
            intensities[1] = glm::clamp(glm::dot(normals[1],lightDir),0.0f,1.0f);
            intensities[2] = glm::clamp(glm::dot(normals[2],lightDir),0.0f,1.0f);

            glm::vec3 ambient(0.15f,0.15f,0.15f);

            glm::vec3 colors[3];
            colors[0] =  ambient + (intensities[0]*diffuse);
            colors[1] =  ambient + (intensities[1]*diffuse);
            colors[2] =  ambient + (intensities[2]*diffuse);

            int width = head_buffer.getWidth();
            int height = head_buffer.getHeight();
            glm::vec2 p0 = clipToViewport(verts[0], width, height);
            glm::vec2 p1 = clipToViewport(verts[1], width, height);
            glm::vec2 p2 = clipToViewport(verts[2], width, height);
            
            head_buffer.drawTriangle(p0.x,p0.y,p1.x,p1.y,p2.x,p2.y,
                    Targa::toTargaFormat(colors[0]),
                    Targa::toTargaFormat(colors[1]),
                    Targa::toTargaFormat(colors[2]));
        }
        Targa::write(filename, head_buffer);
    }
}



