#include <cstdlib>
#include <memory>
template<typename T>
class Buffer2D
{
    private:
    std::unique_ptr<T[]> data;
    int width, height;
    public:
    Buffer2D(int width, int height);
    int getWidth() { return width; }
    int getHeight() { return height; }
    bool isInBounds(int x, int y) { return x >=0 && x < width && y >= 0 && y < height;}
    void boundsAssert(int x, int y);
    T get(int x, int y);
    void set(int x, int y, T value);
    void clear(T value);
    size_t getByteSize();
    T* rawData() { return data.get(); }
};

#include "buffer2d_impl.hpp"
