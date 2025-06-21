#pragma once

#include <optional>
#include <iterator>
#include <type_traits>
#include <cstddef>
#include <functional>
#include <utility>

namespace safe {

template<typename It>
class iterator {
public:
    using traits            = std::iterator_traits<It>;
    using value_type        = typename traits::value_type;
    using reference         = typename traits::reference;
    using pointer           = typename traits::pointer;
    using difference_type   = typename traits::difference_type;
    using iterator_category = typename traits::iterator_category;

    iterator(It begin, It end)
      : begin_(begin), end_(end), current_(begin)
    {}

    iterator(It begin, It end, It current)
      : begin_(begin), end_(end), current_(current)
    {}

    // safe deref: nullopt if at or past end
    std::optional<std::reference_wrapper<value_type>> operator*() const {
        if (current_ != end_) return *current_;
        return std::nullopt;
    }

    // safe arrow: nullopt if at or past end
    std::optional<pointer> operator->() const {
        if (current_ != end_) return std::addressof(*current_);
        return std::nullopt;
    }

    // pre‐increment
    iterator& operator++() {
        if (current_ != end_) ++current_;
        return *this;
    }

    // post‐increment
    iterator operator++(int) {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

    // pre‐decrement
    iterator& operator--() {
        if (current_ != begin_) {
            --current_;
        }
        return *this;
    }

    // post‐decrement
    iterator operator--(int) {
        iterator tmp = *this;
        --*this;
        return tmp;
    }

    // advance by +n or -n, clamped between [begin_,end_]
    iterator& operator+=(difference_type n) {
        if (current_ == end_) return *this;
        std::advance(current_, n);
        if (current_ > end_)   current_ = end_;
        if (current_ < begin_) current_ = begin_;
        return *this;
    }

    iterator& operator-=(difference_type n) {
        return *this += -n;
    }

    // distance between two safe::iterators
    difference_type operator-(iterator const& o) const {
        // if either is "end", we choose distance = 0
        if (current_ == end_ || o.current_ == end_) return 0;
        return std::distance(o.current_, current_);
    }

    // --- friends for arithmetic ---
    friend iterator operator+(iterator it, difference_type n) {
        it += n; return it;
    }
    friend iterator operator+(difference_type n, iterator it) {
        it += n; return it;
    }
    friend iterator operator-(iterator it, difference_type n) {
        it -= n; return it;
    }

    // --- friends for comparisons ---
    friend bool operator==(iterator const& a, iterator const& b) {
        return a.current_ == b.current_;
    }
    friend bool operator!=(iterator const& a, iterator const& b) {
        return !(a == b);
    }
    friend bool operator<(iterator const& a, iterator const& b) {
        return a.current_ < b.current_;
    }
    friend bool operator<=(iterator const& a, iterator const& b) {
        return !(b < a);
    }
    friend bool operator>(iterator const& a, iterator const& b) {
        return b < a;
    }
    friend bool operator>=(iterator const& a, iterator const& b) {
        return !(a < b);
    }

private:
    It begin_, end_, current_;
};

// helpers to inject into range‐based for, etc.
template<typename C>
auto begin(C& c) {
    return iterator<typename C::iterator>(
        c.begin(), c.end(), c.begin());
}
template<typename C>
auto end(C& c) {
    return iterator<typename C::iterator>(
        c.begin(), c.end(), c.end());
}

} // namespace safe
