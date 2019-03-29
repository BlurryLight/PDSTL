#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <cstddef> //for std::nullptr_t
namespace pdstl {
//by SGI_PORT STL

// 5 basic iterators
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bidirectional_iterator_tag : public forward_iterator_tag{};
struct random_access_iterator_tag : public bidirectional_iterator_tag{};

//deprecated in C17
//the reason was explaned by
//https://www.fluentcpp.com/2018/05/08/std-iterator-deprecated/
//we prefer to use  class MyIterator{ using iterator_category = .....} rathter than
//class Myiterator : public std::iteraotr<xxx,xxx>
template <typename Category,typename T,
          typename Distance=std::ptrdiff_t,typename Pointer = T*,typename Reference = T&>
struct iterator
{
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
};
//output_iterator partial specialization
template<>
struct iterator<output_iterator_tag,void,void,void,void>
{
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;
};

template <typename T>
struct input_iterator : public iterator<input_iterator_tag,T>{};
template <typename T>
struct bidirectional_iterator : public iterator<bidirectional_iterator_tag,T>{};
template <typename T>
struct forward_iterator : public iterator<forward_iterator_tag,T>{};
template <typename T>
struct random_access_iterator : public iterator<random_access_iterator_tag,T>{};
template <typename T>
struct output_iterator : public iterator<output_iterator_tag,void,void,void,void>{};

//magic begins
//partial specialization
template <typename iteCat>
struct CategoryMapping
{
    typedef iteCat Tag;
};
template<>
struct CategoryMapping<input_iterator_tag>
{
    typedef input_iterator_tag Tag;
};
template<>
struct CategoryMapping<output_iterator_tag>
{
    typedef output_iterator_tag Tag;
};
template<>
struct CategoryMapping<forward_iterator_tag>
{
    typedef forward_iterator_tag Tag;
};
template<>
struct CategoryMapping<bidirectional_iterator_tag>
{
    typedef bidirectional_iterator_tag Tag;
};
template<>
struct CategoryMapping<random_access_iterator_tag>
{
    typedef random_access_iterator_tag Tag;
};
//embeded mapping iterator_traits
template <typename Iter>
struct iterator_traits
{
    typedef typename Iter::iterator_catogory iterator_category;
//    typedef typename CategoryMapping<originTag>::Tag iterator_category;
    typedef typename Iter::value_type value_type;
    typedef typename Iter::difference_type difference_type;
    typedef typename Iter::pointer pointer;
    typedef typename Iter::reference reference;
};

//partial specialization for pointers
template <typename T>
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
};

template <typename T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};


//trivial part

template <typename T>
random_access_iterator_tag iterator_category(const T*)
{
    return random_access_iterator_tag();
};
template <typename Iter>
typename iterator_traits<Iter>::iterator_category
iterator_category(const Iter&)
{
    typedef typename iterator_traits<Iter>::iterator_category Category;
    return Category();
};



template <typename T>
T* value_type(const T*)
{
    return static_cast<T*>(0); //magic
};
template <typename Iter>
typename iterator_traits<Iter>::value_type*
iterator_category(const Iter&)
{
    return static_cast<typename iterator_traits<Iter>::value_type*>(0);
};


template <typename T>
ptrdiff_t* value_type(const T*)
{
    return static_cast<ptrdiff_t*>(0);
};
template <typename Iter>
typename iterator_traits<Iter>::difference_type*
distance_type(const Iter&)
{
    return static_cast<typename iterator_traits<Iter>::difference_type*>(0);
};




}//pdstl
#endif
