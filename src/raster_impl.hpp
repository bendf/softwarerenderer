#include <utility>
#include <glm/glm.hpp>
#include <tuple>


template<typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}


template<typename OutIter, typename AttrBundle> 
void rasterLine(const AttrBundle &p0, const AttrBundle &p1, OutIter iterator)
{
	//AttrBundle always contains at least getPos() and componentWise * and + operators
   //Assume the p0,p1 sit perfectly centered in some pixels
   glm::ivec2 pos0 = glm::ivec2(p0.getPos());
   glm::ivec2 pos1 = glm::ivec2(p1.getPos());

   bool steep = false;

   if(auto delta = pos1 - pos0; std::abs(delta.y) > std::abs(delta.x))
   {
        steep = true;
        std::swap(pos0.x,pos0.y);
        std::swap(pos1.x,pos1.y);
   }

	bool swapped = false;
   if(pos1.x < pos0.x)
   {
		swapped = true;
		std::swap(pos0, pos1);
   }

   for(auto [delta,pos,error] = std::tuple<glm::ivec2, glm::ivec2, int>{pos1-pos0,pos0,0};  pos.x <= pos1.x; pos.x++, error+=2*delta.y)
   {
        if(std::abs(error) > delta.x)
        {
            error -= sgn(delta.y)*2*delta.x;
            pos.y+= sgn(delta.y);
        }

		//Despite all our fuckery with swapping around x's and ys and and whatnot, our barycentric coordinates
		//will still hold, since we've only performed symmetrical actions, no changes of 
		float v = glm::length(glm::vec2(pos - pos0)) / glm::length(glm::vec2(delta));
		float u = 1.0f - v;
		if(swapped)	
		{	
			std::swap(u,v);
		}
        *iterator++ = (p0 * u) + (p1 * v);
   }

}



template<typename OutIter, typename AttrBundle> 
void rasterTriangle(glm::vec3 const &p0, glm::vec3 const &p1, glm::vec3 const &p2, OutIter iterator)
{
    glm::vec3 botLeft = glm::min(glm::min(p0,p1),p2);
    glm::vec3 topRight = glm::max(glm::max(p0,p1),p2);

    for(int x = std::round(botLeft.x); x < std::round(topRight.x); x++)
    {
        for(int y = std::round(botLeft.y); y < std::round(topRight.y); y++)
        {
            glm::vec3 bc = barycentricCoordinatesOrtho(p0,p1,p2, glm::vec2(x,y));  
            if((bc.x < 1.0f && bc.x >0.0f) && (bc.y < 1.0f && bc.y > 0.0f) && (bc.z < 1.0f && bc.z > 0.0f))
            {
                *iterator++ = (p0 * bc.x) + (p1 * bc.y) + (p2 * bc.z);
            }
        }
    }

}
