#include <cstdlib>
#include <memory>
#ifndef BUFFER_2D_H
#define BUFFER_2D_H


void barycentricCoordinates(float x0, float y0, float x1, float y1, float x2, float y2, float x, float y, float& u, float &v, float&w);

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
    size_t getByteSize();
    T* rawData() { return data.get(); }

    T get(int x, int y);
    void set(int x, int y, T value);

    void clear(T value);
    void drawLine(int x0, int y0, int x1, int y1, T value);
    void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, T value);

};

#include "buffer2d_impl.hpp"
#endif

