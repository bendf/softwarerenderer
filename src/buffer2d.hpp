#include <cstdlib>
template<typename T>
class Buffer2D
{
    private:
    std::unique_ptr<T[]> data;
    int width;
    int height;
    public:
    int getWidth() { return width; }
    int getHeight() { return height; }
    T get(int x, int y);
    void set(int x, int y, T value);
    size_t get_byte_size();
    Buffer2D(int width, int height);
};

//Need to declare specialization of template class here, so the object file built contains code for it.
//Since buffer2d_test will need it.
template class Buffer2D<int>;

