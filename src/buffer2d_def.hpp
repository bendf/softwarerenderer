#include <memory>
#include <sstream>

template<typename T>
class Buffer2D
{
    private:
    std::unique_ptr<T[]> data;
    int _width, _height;

    public:
    Buffer2D(int width, int height);

    bool isInBounds(int x, int y) const { return x >=0 && x < _width && y >= 0 && y < _height;}

    int width() const { return _width; }
    int height() const { return _height; }
    size_t byteSize() const { return _width * _height * sizeof(T);}
    T* rawPtr() const { return data.get(); }

    T get(int x, int y) const;
    bool set(int x, int y, T value);
    void clear(T value);

};


