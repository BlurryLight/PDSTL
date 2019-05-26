#ifndef PAIR_HPP
#define PAIR_HPP
#include "utility.hpp"
#include <algorithm> //std::swap
namespace pdstl {

template<typename T1, typename T2>
struct pair
{
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;
    pair()
        : first()
        , second()
    {}
    //标准要求必须T1,T2必须要is_default_constrctible_v,这里没有处理,报错一律当ub
    //标准要求constexpr，也不处理
    pair(const T1 &x, const T2 &y)
        : first(x)
        , second(y)
    {}
    pair(const pair &other) = default;
    pair(pair &&) = default;
    template<typename U, typename V>
    pair(const pair<U, V> &other)
        : first(other.first)
        , second(other.second)
    {}

    template<typename U, typename V>
    pair(pair<U, V> &&other)
        : first(other.first)
        , second(other.second)
    {}

    pair &operator=(const pair &other)
    {
        this->first = other.first;
        this->second = other.second;
        return *this;
    }
    template<typename U, typename V>
    pair &operator=(const pair<U, V> &other)
    {
        this->first = other.first;
        this->second = other.second;
        return *this;
    }

    pair &operator=(pair &&other)
    {
        this->first = pdstl::forward<first_type>(other.first);
        this->second = pdstl::forward<second_type>(other.second);
        return *this;
    }

    template<typename U, typename V>
    pair &operator=(pair<U, V> &&other)
    {
        this->first = pdstl::forward<U>(other.first);
        this->second = pdstl::forward<V>(other.second);
        return *this;
    }

    void swap(pair &other) noexcept
    {
        std::swap(first, other.first);
        std::swap(second, other.second);
    }
};
template<typename T1, typename T2>
bool operator==(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return (lhs.first == rhs.first) && (lhs.second == rhs.second);
}

template<typename T1, typename T2>
bool operator<(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return ((lhs.first < rhs.first) || (!(rhs.first < lhs.first) && (lhs.second < rhs.second)));
}

template<typename T1, typename T2>
bool operator>(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return rhs < lhs;
}

template<typename T1, typename T2>
bool operator!=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return !(lhs == rhs);
}

template<typename T1, typename T2>
bool operator<=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return (lhs < rhs || lhs == rhs);
}

template<typename T1, typename T2>
bool operator>=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return (lhs > rhs || lhs == rhs);
}

template<typename T1, typename T2>
void swap(pair<T1, T2> &x, pair<T1, T2> &y)
{
    x.swap(y);
}

template<typename T1, typename T2>
pair<T1, T2> make_pair(T1 t, T2 u)
{
    return pair<T1, T2>(t, u);
}

} // namespace pdstl
#endif
