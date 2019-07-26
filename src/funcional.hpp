#pragma once

namespace pdstl {

template <typename Argument1,typename Argument2,typename Result>
struct binary_funcion
{
    typedef Argument1 first_argument_type;
    typedef Argument2 second_argument_type;
    typedef Result result_type;
};
template <class T>
struct less : public binary_funcion<T,T,bool>
{
    constexpr bool operator()(const T& a, const T&b) const
    {
        return a < b;
    }
};

}//pdstl
