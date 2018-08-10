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
    void set(int x, int y, T value);
    void clear(T value);

};

class buffer2d_out_of_range : public std::exception 
{
    std::string msg;
    public:
    buffer2d_out_of_range(int x, int y, int width, int height)  
    {
        std::stringstream ss; 
        ss << "Attempt to access location (" << x << "," << y
           << ") in buffer of size (" << width << "," << height << ")";
        msg = ss.str(); 
    }

    virtual char const * what()
    {
        return msg.c_str();
    }
};


