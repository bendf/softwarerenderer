#ifndef ATTRIB_BUNDLE_H
#define ATTRIB_BUNDLE_H
#include <tuple>

template<typename ...Ts>
class Attributes : public std::tuple<Ts...>
{
    public:
    Attributes(Ts ...args) : std::tuple<Ts...>(args...) {}
    Attributes() : std::tuple<Ts...>() {}

    template<size_t ...Ns> 
    Attributes mult_impl(float o, std::index_sequence<Ns...>) const
    {
        return Attributes((o*std::get<Ns>(*this))...);
    }
    Attributes operator*(float o) const
    {
        return mult_impl(o,std::index_sequence_for<Ts...>{});
    }

    template<size_t ...Ns> 
    Attributes add_impl(const Attributes& other, std::index_sequence<Ns...>) 
    {
        return Attributes((std::get<Ns>(*this) + std::get<Ns>(other))...);
    }
    Attributes operator+(const Attributes& other) 
    {
        return add_impl(other, std::index_sequence_for<Ts...>{});
    }

    auto pos() const 
    {
        return std::get<0>(*this);
    }
};

#endif
