
template<typename T>
void renderTriangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec2 vt0, glm::vec2 vt1, glm::vec2 vt2, Buffer2D<T>* buffer, 
        Buffer2D<float>* depthBuffer, Buffer2D<T>* textureBuffer)
{
    if(depthBuffer != nullptr && (buffer->getWidth() != depthBuffer->getWidth() || buffer->getHeight() != depthBuffer->getHeight()))
    {
        throw std::invalid_argument("Depth buffer size does not match main buffer size");
    }
    glm::ivec2 vMin = glm::min(glm::min(p0,p1),p2);
    glm::ivec2 vMax = glm::max(glm::max(p0,p1),p2);

    for(int x = vMin.x; x <=vMax.x; x++)
    {
        for(int y = vMin.y; y <=vMax.y; y++)
        {
            if(buffer->isInBounds(x,y))
            {
                glm::vec3 bc = barycentricCoordinates(glm::vec3(p0.x,p0.y,0),glm::vec3(p1.x,p1.y, 0),
                glm::vec3(p2.x,p2.y,0), glm::vec3(x,y,0));
                float z = (bc.x * p0 + bc.y * p1 + bc.z * p2).z;
                   
                if(bc.x >= 0 && bc.y >= 0 && bc.z >= 0)
                {
                    glm::vec2 vt = (bc.x * vt0) + (bc.y * vt1)  + (bc.z * vt2);
                    T texValue = textureBuffer->get(vt.x * textureBuffer->getWidth(), vt.y * textureBuffer->getHeight());
                    if(depthBuffer == nullptr)
                    {
                        buffer->set(x,y,texValue);
                    } 
                    else if( z < depthBuffer->get(x,y))
                    {
                        depthBuffer->set(x,y,z);
                        buffer->set(x,y,texValue);
                    }
                }
            
            }
        }
    }

}
