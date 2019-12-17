#ifndef MAP_HPP
#define MAP_HPP

#include "allocator.h"
#include "avl_tree_impl.hpp"
#include "funcional.hpp"
#include "iterator.hpp"
#include "pair.hpp"
#include "utility.hpp"
#include <initializer_list>
#include <iterator>
namespace pdstl {

template<typename Iter>
using isInputIterator = typename std::enable_if<
    std::is_convertible<typename pdstl::iterator_traits<Iter>::iterator_category,
                        pdstl::input_iterator_tag>::value>;
template<typename Key,
         typename T,
         typename Compare = less<Key>,
         typename Alloc = Allocator<pair<const Key, T>>>
class map
{
    typedef Key key_type;
    typedef T mapped_type;
    typedef pair<const Key, T> value_type;

    struct key_compare
    {
        bool operator()(const value_type&x,const value_type&y,Compare comp = Compare())
        {
            return comp(x.first,y.first);
        }
    };

    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef typename Alloc::pointer pointer;
    typedef typename Alloc::const_pointer const_pointer;

    typedef AVLTree<value_type,key_compare,key_type> tree_type;
    typedef typename tree_type::iterator iterator;
    typedef typename tree_type::const_iterator const_iterator;
    typedef typename tree_type::node_type node_type;

private:
    tree_type avltree_;

public:
    map();
    map(const map &other);
    map(map &&other);
    map(std::initializer_list<value_type> ilist);
    template<typename InputIt, typename = isInputIterator<InputIt>>
    map(InputIt first, InputIt last);

    map &operator=(const map &other);
    map &operator=(map &&other);

    ~map()
    {

    }

    T &at(const Key &key);
    const T &at(const Key &key) const;

    T &operator[](const Key &key);

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    size_type size()const {return this->avltree_.size();}
    bool empty() const;

    void clear();

    //modifiers
    pair<iterator, bool> insert(const value_type &value);
    template<class P>
    pair<iterator, bool> insert(P &&value);

    iterator insert(const_iterator hint, const value_type &value);

protected:
    //map should be unique
//    bool isKeyUnique(const Key &key)
//    {
//        //        return avltree_.find()
//    }

};

template<typename Key, typename T, typename Compare, typename Alloc>
map<Key,T,Compare,Alloc>::map()
{
    this->avltree_ = tree_type();
}

template<typename Key, typename T, typename Compare, typename Alloc>
map<Key,T,Compare,Alloc>::map(const map& other)
{
    this->avltree_ = tree_type();
    this->avltree_ = other.avltree_;
}

template<typename Key, typename T, typename Compare, typename Alloc>
typename map<Key,T,Compare,Alloc>::iterator
map<Key,T,Compare,Alloc>::begin()
{
    return this->avltree_.begin();
}

template<typename Key, typename T, typename Compare, typename Alloc>
pair<typename map<Key,T,Compare,Alloc>::iterator,bool>
map<Key,T,Compare,Alloc>::insert(const value_type& value)
{
    auto it = this->avltree_.findByKey(value.first) ;
    if(it != this->avltree_.end())
        return pair<typename map<Key,T,Compare,Alloc>::iterator,bool>(it,false);
    it = this->avltree_.insert(value);
    return pair<typename map<Key,T,Compare,Alloc>::iterator,bool>(it,true);
}

} //namespace pdstl

#endif
