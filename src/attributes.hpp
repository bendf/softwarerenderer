#ifndef ATTRIB_BUNDLE_H
#define ATTRIB_BUNDLE_H
#include <tuple>

template<typename ...Ts>
class Attributes : public std::tuple<Ts...>
{
    public:
    Attributes(Ts ...args) : std::tuple<Ts...>(args...) {}
    template<size_t ...Ns> Attributes mult(float o, std::index_sequence<Ns...> seq) const
    {
        return Attributes((o*std::get<Ns>(*this))...);
    }
    Attributes operator*(float o) const
    {
        return mult(o,std::index_sequence_for<Ts...>{});
    }

    template<size_t ...Ns> 
    Attributes add(const Attributes& other, std::index_sequence<Ns...> seq) 
    {
        return Attributes((std::get<Ns>(*this) + std::get<Ns>(other))...);
    }
    Attributes operator+(const Attributes& other) 
    {
        return add(other, std::index_sequence_for<Ts...>{});
    }

    auto pos() const -> decltype(std::get<0>(*this)) 
    {
        return std::get<0>(*this);
    }
};

#endif
