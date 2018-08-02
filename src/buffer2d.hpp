#ifndef BUFFER_2D_H
#define BUFFER_2D_H
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

    bool isInBounds(int x, int y) { return x >=0 && x < width && y >= 0 && y < height;}
    void boundsAssert(int x, int y);

    int getWidth() { return width; }
    int getHeight() { return height; }
    size_t getByteSize() { return width * height * sizeof(T);}
    T* rawData() { return data.get(); }

    T get(int x, int y);
    void set(int x, int y, T value);
    void clear(T value);
};

#include "buffer2d_impl.hpp"
#endif

