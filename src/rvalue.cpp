#include <iostream>
class X {
    public:
    X(int) { std::cout << "Int Constructor\n";}
    X() { std::cout << "Normal Constructor\n";}
    ~X() { std::cout << "Normal Destructor\n";}
    X(const X& o) {std::cout << "Copy Constructor\n";}
    X(X&& o) {std::cout << "Move Constructor\n";}

};

X foo()
{
    return X();
}

int main()
{
    X x;
    X y(x);
    X z(std::move(X(1)));

}


//std move turns lvalue reference to rvalue reference X& -> X&&
template<typename T>
void swap(T& a, T& b)
{
    T tmp(std::move(a));
    a = std::move(b);
    b = std::move(tmp);

}
