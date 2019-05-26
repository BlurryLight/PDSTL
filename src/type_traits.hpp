#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

namespace pdstl {

//copy from cppreference
template<typename T, T v>
struct integral_constant
{
    static constexpr T value = v;
    typedef T value_type;
    typedef integral_constant type;
    constexpr operator value_type() const { return value; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

//another possible implementation
/*
 * struct true_type{}:
 * struct false_type{};
 */

//an vivid example to show type_traits trick
// from libstdc++
// is_integral

template<typename T>
struct is_integral : public false_type
{};
template<>
struct is_integral<bool> : public true_type
{};
template<>
struct is_integral<char> : public true_type
{};
template<>
struct is_integral<signed char> : public true_type
{};
template<>
struct is_integral<unsigned char> : public true_type
{};
template<>
struct is_integral<short> : public true_type
{};
template<>
struct is_integral<unsigned short> : public true_type
{};
template<>
struct is_integral<int> : public true_type
{};
template<>
struct is_integral<unsigned int> : public true_type
{};
template<>
struct is_integral<long> : public true_type
{};
template<>
struct is_integral<unsigned long> : public true_type
{};
template<>
struct is_integral<long long> : public true_type
{};
template<>
struct is_integral<unsigned long long> : public true_type
{};

//type_traits
//not correspond with current standards
//from SGI STL
template<typename type>
struct __type_traits
{
    typedef true_type this_dummy_member_must_be_first;

    typedef false_type has_trivial_default_constructor;
    typedef false_type has_trivial_copy_constructor;
    typedef false_type has_trivial_assignment_operator;
    typedef false_type has_trivial_destructor;
    typedef false_type is_POD_type;
};

template<>
struct __type_traits<int>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};

template<>
struct __type_traits<bool>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};

template<>
struct __type_traits<char>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};

template<>
struct __type_traits<unsigned char>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};
template<>
struct __type_traits<signed char>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};

template<>
struct __type_traits<short>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};
template<>
struct __type_traits<unsigned short>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};
template<>
struct __type_traits<long>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};
template<>
struct __type_traits<unsigned long>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};
template<>
struct __type_traits<unsigned long long>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};
template<>
struct __type_traits<long long>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};
template<>
struct __type_traits<float>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};
template<>
struct __type_traits<double>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};

template<>
struct __type_traits<long double>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};
template<typename T>
struct __type_traits<T *>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};
template<typename T>
struct __type_traits<const T *>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};
template<>
struct __type_traits<char *>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};
template<>
struct __type_traits<unsigned char *>
{
    typedef true_type has_trivial_default_constructor;
    typedef true_type has_trivial_copy_constructor;
    typedef true_type has_trivial_assignment_operator;
    typedef true_type has_trivial_destructor;
    typedef true_type is_POD_type;
};
};     // namespace pdstl
#endif // TYPE_TRAITS_HPP
