#include <utility>

template<typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}


template<typename O> void rasterLine(
        glm::vec3 const &p0, //screen space
        glm::vec3 const &p1,  //screen space
        O iterator)
{
   //Assume the p0,p1 sit perfectly centered in some pixels
   int x0 = p0.x, y0 = p0.y;
   int x1 = p1.x, y1 = p1.y;

   bool steep = false;
   if(std::abs(y1 - y0) > std::abs(x1 - x0))
   {
        steep = true;
        std::swap(x0,y0);
        std::swap(x1,y1);
   }
   if(x1 < x0)
   {
        std::swap(x0,x1);
        std::swap(y0,y1);
   }

   int deltaX = (x1 - x0);
   int deltaY = (y1 - y0);


   for(int x = x0, y = y0, error = 0;  x <= x1; x++, error+=2*deltaY)
   {
        if(std::abs(error) > deltaX)
        {
            error -= sgn(deltaY)*2*deltaX;
            y+= sgn(deltaY);
        }
        if(steep)
        {
            *iterator++ = std::make_pair(y,x);
        } 
        else 
        {
            *iterator++ = std::make_pair(x,y);
        }
   }

}
