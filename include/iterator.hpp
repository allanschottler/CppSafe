#pragma once

#include "optional.hpp"
#include <iterator>
#include <type_traits>
#include <cstddef>
#include <functional>
#include <utility>

namespace safe {

template<typename It>
struct iterator {
    using traits            = std::iterator_traits<It>;
    using value_type        = typename traits::value_type;
    using reference         = typename traits::reference;
    using pointer           = typename traits::pointer;
    using difference_type   = typename traits::difference_type;
    using iterator_category = typename traits::iterator_category;
    using deref_type        = optional<std::reference_wrapper<value_type>>;

    iterator(It begin, It end)
      : begin_(begin), end_(end), current_(begin)
    {}

    iterator(It begin, It end, It current)
      : begin_(begin), end_(end), current_(current)
    {}

    // safe deref: nullopt if at or past end
    deref_type operator*() const {
        if (!out_of_range && current_ != end_)
            return std::ref(*current_);
        return std::nullopt;
    }

    struct arrow_proxy {
        deref_type opt;
        deref_type* operator->() 
        { return &opt; }
    };

    arrow_proxy operator->() {
        arrow_proxy p;
        if (!out_of_range && current_ != end_)
            p.opt = std::ref(*current_);
        return p;
    }

    // pre‐increment
    iterator& operator++() {
        if (out_of_range < 0)
            out_of_range = 0; // reset
        if (current_ == end_)
            out_of_range = 1; // mark as out of range
        else
            ++current_;
        return *this;
    }

    // post‐increment
    iterator operator++(int) {
        iterator tmp = *this;
        if (out_of_range < 0)
            out_of_range = 0; // reset
        if (current_ == end_)
            out_of_range = 1; // mark as out of range
        else
            ++*this;
        return tmp;
    }

    // pre‐decrement
    iterator& operator--() {
        if (out_of_range > 0)
            out_of_range = 0; // reset
        if (current_ == begin_)
            out_of_range = -1; // mark as out of range
        else
            --current_;
        return *this;
    }

    // post‐decrement
    iterator operator--(int) {
        iterator tmp = *this;
        if (out_of_range > 0)
            out_of_range = 0; // reset
        if (current_ == begin_)
            out_of_range = -1; // mark as out of range
        else 
            --*this;
        return tmp;
    }

    iterator& operator+=(difference_type n) {
        std::advance(current_, n);
        return *this;
    }

    iterator& operator-=(difference_type n) 
    { return *this += -n; }

    // distance between two safe::iterators
    difference_type operator-(iterator const& o) const {
        // if either is "end", we choose distance = 0
        if (current_ == end_ || o.current_ == end_) return 0;
        return std::distance(o.current_, current_);
    }
    
        auto begin() const { return begin_; }
        auto end() const   { return end_; }

    // --- friends for arithmetic ---
    friend iterator operator+(iterator it, difference_type n) { it += n; return it; }
    friend iterator operator+(difference_type n, iterator it) { it += n; return it; }
    friend iterator operator-(iterator it, difference_type n) { it -= n; return it; }

    // --- friends for comparisons ---
    friend bool operator==(iterator const& a, iterator const& b) { return a.current_ == b.current_; }
    friend bool operator!=(iterator const& a, iterator const& b) { return !(a == b); }
    friend bool operator<(iterator const& a, iterator const& b)  { return a.current_ < b.current_; }
    friend bool operator<=(iterator const& a, iterator const& b) { return !(b < a); }
    friend bool operator>(iterator const& a, iterator const& b)  { return b < a; }
    friend bool operator>=(iterator const& a, iterator const& b) { return !(a < b); }

private:
    friend struct unsafe_ref_get;
    It begin_, end_, current_;
    char out_of_range = 0;
};

} // namespace safe
