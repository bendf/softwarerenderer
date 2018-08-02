#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>

#include "render.hpp"
#include "targa.hpp"

namespace Catch {
	template<>
    struct StringMaker<glm::vec3> {
    	static std::string convert( glm::vec3 const& value ) {
            
            std::stringstream s;
            s << "(" << value.x << "," << value.y << "," << value.z << ")";
        	return s.str();
        }
    };
}


TEST_CASE("Render", "[render buffer2d]")
{

}
