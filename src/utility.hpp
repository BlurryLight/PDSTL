#ifndef UTILITY_HPP
#define UTILITY_HPP
/*
 * pair has been moved to pair.hpp
 */
namespace pdstl {
template<class T>
struct remove_reference
{
    typedef T type;
};
template<class T>
struct remove_reference<T &>
{
    typedef T type;
};
template<class T>
struct remove_reference<T &&>
{
    typedef T type;
};

template<class T>
typename remove_reference<T>::type &&move(T &&a) noexcept
{
    typedef typename remove_reference<T>::type &&RvalRef;
    return static_cast<RvalRef>(a);
}

//from libc++
template<typename T>
T &&forward(typename remove_reference<T>::type &t)
{
    return static_cast<T &&>(t);
}

template<typename T>
T &&forward(typename remove_reference<T>::type &&t)
{
    return static_cast<T &&>(t);
}

} // namespace pdstl
#endif
