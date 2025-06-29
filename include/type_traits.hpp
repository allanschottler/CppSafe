#pragma once
#include "iterator.hpp"
#include "vector.hpp"

namespace safe {
    template<class T>
    struct is_safe;

    template<class T>
    struct is_safe : std::false_type {};

    template<class T>
    struct is_safe<safe::iterator<T>> : std::true_type {};

    template<class T>
    struct is_safe<safe::vector<T>> : std::true_type {};
}