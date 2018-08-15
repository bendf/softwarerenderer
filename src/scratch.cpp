#include <tuple>
#include <iostream>
#include <utility>

/*
void print() 
{
    std::cout << "empty overload reached!\n";
}

template<typename S, typename ...Ts>
void print(S first, Ts ...rest)
{
    std::cout << first << "\n";
    print(rest...);
}

template<typename T, size_t ...N>
auto print(T tuple) ->  decltype (std::get<0>(tuple), void())
{
    std::cout << "tuple overload\n";
    print(std::get<N>(tuple)...);
}



int main()
{
    auto t0 = std::make_tuple("Hello", "There", "Bob", "How's", "it", "going?"); 
    print<decltype(t0),0,1,2>(t0);
}
*/

/*
template<int N>
int realise()
{
    return N;
}

void print() 
{
    std::cout << "Empty print overload\n";
}

template<typename S, typename ...T>
void print(S first, T... rest)
{
    std::cout << first << "\n";
    print(rest...);
}

template<int ...N>
void printIntPack()
{
    std::cout << "printIntPack\n";
   print(realise<N>()...); 
}

int main()
{
    printIntPack<1,7,5>();
}
*/

/*
template<int N>
int realise()
{
    return N;
}

template<int ...Ns> struct sequence{};
template<int ...Ns> struct sequence_gen;

template<int I, int ...Ns>
struct sequence_gen<I, Ns...>
{

    using type = typename sequence_gen<I-1, I-1, Ns...>::type;

};

template<int ...Ns>
struct sequence_gen<0, Ns...>
{
    using type = sequence<Ns...>;
};


template<int N>
using sequence_t = typename sequence_gen<N>::type;

void print()
{
}
template<typename S, typename ... Ts>
void print(S first, Ts ...rest)
{
    std::cout << first << "\n";
    print(rest...);
}

//Matches a sequence with some non-zero number of template args
template<int ...Ns>
void print(sequence<Ns...> seq)
{
    print(realise<Ns>()...);    
}

int main()
{
    print(sequence_t<0>{});
}

*/

/*
template<int ...Ns> struct sequence{};
template<int ...Ns> struct sequence_gen;

template<int I, int ...Ns> 
struct sequence_gen<I, Ns...>
{
    using type = typename sequence_gen<I-1, I-1, Ns...>::type;
};

template<int ...Ns>
struct sequence_gen<0, Ns...>
{
    using type = sequence<Ns...>;
};

template<int N>
using sequence_t = typename sequence_gen<N>::type;


*/

/*
void print() {}

template<typename T, typename ...Ts>
void print(T first, Ts ...rest)
{
    std::cout << first << "\n";
    print(rest...);
}

template<typename T, size_t ...Is>
void print_tuple(T tuple, std::index_sequence<Is...> seq)
{
    print(std::get<Is>(tuple)...);
}

template<typename ...Ts>
void print(std::tuple<Ts...> tuple)
{
    print_tuple(tuple, std::index_sequence_for<Ts...>{});
}


int main()
{
    auto t0 = std::make_tuple("Hello", "there", "bob", "yo", "I", "wanna", "be", "the", "very", "best", "like", "no-one", "ever", "was", 7);
    print(t0);

}

*/
template<typename ...Ts>
class AttribBundle : public std::tuple<Ts...>
{
    public:
    AttribBundle(Ts ...args) : std::tuple<Ts...>(args...) {}
    template<size_t ...Ns> AttribBundle mult(const float o, std::index_sequence<Ns...> seq)
    {
        return AttribBundle((o*std::get<Ns>(*this))...);
    }
    AttribBundle operator*(const float o)
    {
        return mult(o,std::index_sequence_for<Ts...>{});
    }

    template<size_t ...Ns> 
    AttribBundle add(const AttribBundle& other, std::index_sequence<Ns...> seq)
    {
        return AttribBundle((std::get<Ns>(*this) + std::get<Ns>(other))...);
    }
    AttribBundle operator+(const AttribBundle& other)
    {
        return add(other, std::index_sequence_for<Ts...>{});
    }
};

int main()
{
    AttribBundle<float,float> a(3.0,2.0);
    AttribBundle<float,float> b(1.0, 5.0f);
    auto c = (a + b) * 5.0f;
    std::cout << std::get<0>(c) << "," << std::get<1>(c) << "\n";
}
