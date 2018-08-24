#include <iostream>
#include <vector>
#include <array>
#include <glm/glm.hpp>

struct B
{
    glm::vec3 v;
};

std::ostream& operator<<(std::ostream& stream, const B& b)
{
    return  stream << b.v.x << "\n";
}

int main()
{
    std::vector<std::array<B,3>> c;
    c.resize(7);
    std::cout << c[0][0];
}
