#pragma once

#include "iterator.hpp"
#include "type_traits.hpp"
#include "unsafe.hpp"
#include <algorithm>

namespace safe {

template<class It, class T>
iterator<It> find(It first, It last, const T& value) 
{ return {first, last, std::find(first, last, value)}; }

template<class It, class Pred>
iterator<It> find_if(It first, It last, Pred pred) 
{ return {first, last, std::find_if(first, last, pred)}; }

template<class It, class Pred>
iterator<It> find_if_not(It first, It last, Pred pred) 
{ return {first, last, std::find_if_not(first, last, pred)}; }

template<class It, class T>
iterator<It> lower_bound(It first, It last, const T& value) 
{ return {first, last, std::lower_bound(first, last, value)}; }

template<class It, class F>
void for_each(It first, It last, F&& func)
requires safe::is_safe<It>::value {
    for(auto it = first; it != last; ++it)
        func(unsafe_ref_get<It>{}(it));
}

template<class Container, class F>
void for_each(Container& c, F&& func)
requires safe::is_safe<Container>::value {
    for(auto it = c.begin(); it != c.end(); ++it)
        func(unsafe_ref_get<typename Container::iterator>{}(it));
}

} // namespace safe
