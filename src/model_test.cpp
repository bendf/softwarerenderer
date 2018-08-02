#define CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS
#include <catch.hpp>
#include "model.hpp"
#include <sstream>


namespace Catch {
	template<>
    struct StringMaker<glm::vec3> {
    	static std::string convert( glm::vec3 const& v ) {
            std::stringstream ss;
            ss << "(" << v.x << "," << v.y << "," << v.z << ")";
            return ss.str();
        }
    };


    template<>
    struct StringMaker<glm::vec2> {
    	static std::string convert( glm::vec2 const& v ) {
            std::stringstream ss;
            ss << "(" << v.x << "," << v.y <<  ")";
            return ss.str();
        }
    };

}

TEST_CASE("model", "[model obj io]")
{
    std::vector<Model::AttribBundle>  model = Model::LoadAll("../obj/african_head.obj");
    CHECK(model.size() % 3 == 0);
    CHECK(model.size() == 7476);
    glm::vec3 zero(0.0f,0.0f,0.0f);
    CHECK(model[0].position  != zero);
    CHECK(model[0].uv != glm::vec2(0.0f,0.0f));
    CHECK(model[0].normal != zero);
    CHECK_THROWS(model.at(7476));
    CHECK_NOTHROW(model.at(7475));
}
