#ifndef ATTRIB_BUNDLE_H
#define ATTRIB_BUNDLE_H
#include <tuple>

template<typename Pos, typename... Ts>
class AttrTuple : public std::tuple<Pos, Ts...>
{
    private:
    template<size_t ...Ns> 
    AttrTuple mult_impl(float s, std::index_sequence<Ns...>) const
    {
        return AttrTuple((s*std::get<Ns>(*this))...);
    }

    template<size_t ...Ns> 
    AttrTuple add_impl(const AttrTuple& other, std::index_sequence<Ns...>) const
    {
        return AttrTuple((std::get<Ns>(*this) + std::get<Ns>(other))...);
    }

    public:
    AttrTuple(Pos pos,Ts ...args) : std::tuple<Pos,Ts...>(pos, args...) {}
    AttrTuple() : std::tuple<Pos,Ts...>() {}

        AttrTuple operator*(float s) const
    {
        return mult_impl(s,std::index_sequence_for<Pos,Ts...>{});
    }

       AttrTuple operator+(const AttrTuple& other) const
    {
        return add_impl(other, std::index_sequence_for<Pos,Ts...>{});
    }

    Pos pos() const 
    {
        return std::get<0>(*this);
    }
};

#endif
