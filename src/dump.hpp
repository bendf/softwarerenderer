/*
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
*/

    /*
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
        renderTriangle(glm::vec3(0,0,0), glm::vec3(400,400,0), glm::vec3(400,0,0), yellow, yellow, yellow, &tri_buffer);
        renderTriangle(glm::vec3(0,400,0), glm::vec3(100,390,0), glm::vec3(50,150,0), yellow, yellow, yellow, &tri_buffer);
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
    SECTION(" Triangle Head  - visual inspection")
    {
        Buffer2D<Targa::TargaFormat, 1000, 1000> head_buffer;
        Buffer2D<float, 1000,1000> z_buffer;
        z_buffer.clear(1.0f);
        head_buffer.clear({0,0,0});
        const char* filename = "images/head_triangle_lit.tga";
        Model m;
        m.LoadAll("../obj/african_head.obj");
        Buffer2D<Targa::TargaFormat> textureBuffer = Targa::read("../obj/african_head_diffuse.tga");
        
        
        for(int i = 0; i < m.getNumTriangles(); i++)
        {
            Triangle t = m.triangleAt(i);
            glm::vec3 verts[3]; 
            verts[0] = m.vertexAt(t.a.posIndex);
            verts[0].z *=-1.0f;
            verts[1] = m.vertexAt(t.b.posIndex);
            verts[1].z *=-1.0f;
            verts[2] = m.vertexAt(t.c.posIndex);
            verts[2].z *=-1.0f;

            glm::vec2 uvs[3];
            uvs[0] = m.UVAt(t.a.uvIndex);
            uvs[1] = m.UVAt(t.b.uvIndex);
            uvs[2] = m.UVAt(t.c.uvIndex);


            glm::vec3 faceNormal = glm::normalize(glm::cross(verts[1] - verts[0], verts[2] - verts[0]));
            if(faceNormal.z <= 0)
            {
                continue;
            }
            glm::vec3 diffuse(0.7f, 0.5f, 0.4f);
            glm::vec3 lightDir = glm::normalize(glm::vec3(0.0,0.0,1));

            glm::vec3 ambient(0.15f,0.15f,0.15f);
            glm::vec3 color = ambient + (diffuse * glm::clamp(dot(faceNormal, lightDir),0.0f, 1.0f));


            int width = head_buffer.getWidth();
            int height = head_buffer.getHeight();
            glm::vec3 p0 = clipToViewport(verts[0], width, height);
            glm::vec3 p1 = clipToViewport(verts[1], width, height);
            glm::vec3 p2 = clipToViewport(verts[2], width, height);
            
            renderTriangle(p0,p1,p2, uvs[0], uvs[1], uvs[2],
                    &head_buffer, &z_buffer, &textureBuffer);
        }

        Buffer2D<Targa::TargaFormat> depth_targa(1000,1000);
        for(int x = 0; x < depth_targa.getWidth(); x++)
        {
            for(int y = 0; y < depth_targa.getHeight(); y++)
            {
                float f = z_buffer.get(x,y);
                uint8_t v = (2.0f - (f+1.0f))*(255.0f/2.0f);
               Targa::TargaFormat s = {v,v,v};
               depth_targa.set(x,y,s); 
            }
        }
        Targa::write(filename, head_buffer);
        Targa::write("images/head_triangle_depth.tga", depth_targa);
        Targa::write("images/diffuse_tx.tga", textureBuffer);
    }
*/


