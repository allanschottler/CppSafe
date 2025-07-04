#pragma once
#include "memory.hpp"
#include "optional.hpp"
#include "iterator.hpp"

namespace safe {

struct unsafe_ref_get
{
    template<class T>
    auto& operator()(pointer<T>& ptr) const
    { return *ptr.ptr_; }

    template<class T>
    auto& operator()(iterator<T>& itr) const
    { return *itr.current_; }

    template<class T>
    auto& operator()(optional<T>& opt) const
    { return *opt._value; }
};
} 