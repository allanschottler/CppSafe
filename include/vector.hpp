#pragma once

#include "iterator.hpp"
#include <vector>
#include <cstddef>

namespace safe {

// safe_vector
template<class T, class Alloc = std::allocator<T>>
class vector {
private:
    std::vector<T,Alloc> data;
public:
    using value_type      = T;
    using size_type       = typename std::vector<T,Alloc>::size_type;
    using reference       = typename std::vector<T,Alloc>::reference;
    using const_reference = typename std::vector<T,Alloc>::const_reference;
    using iterator        = safe::iterator<typename std::vector<T,Alloc>::iterator>;
    using const_iterator  = safe::iterator<typename std::vector<T,Alloc>::const_iterator>;

    vector() = default;
    explicit vector(size_type n, const T& v = T()) { resize(n, v); }
    explicit vector(const std::vector<T,Alloc>& v) : data(v) {}
    explicit vector(std::vector<T,Alloc>&& v) : data(std::move(v)) {}
    vector(std::initializer_list<T> il) : data(il) {}
    template<class It> vector(It first, It last) : data(first,last) {}

    void reserve(size_type n) { 
        try {
            data.reserve(n);
        } catch (const std::bad_alloc&) {
            // Do nothing, just avoid throwing
        }
    }
    void resize(size_type n, const T& v = T()) { 
        try {
            data.resize(n, v);
        } catch (const std::bad_alloc&) {
            // Do nothing, just avoid throwing
        }
    }
    void clear() noexcept { data.clear(); }

    void push_back(const T& v) { data.push_back(v); }
    size_type size()   const noexcept { return data.size(); }
    bool empty()       const noexcept { return data.empty(); }

    // safe access via optional
    optional<std::reference_wrapper<value_type>> at(size_type i) noexcept {
        return i < data.size() ? optional<std::reference_wrapper<value_type>>{data[i]} : std::nullopt;
    }
    optional<std::reference_wrapper<const value_type>> at(size_type i) const noexcept {
        return i < data.size() ? optional<std::reference_wrapper<const value_type>>{data[i]} : std::nullopt;
    }

    auto operator[](size_type i) noexcept { return at(i); }
    auto operator[](size_type i) const noexcept { return at(i); }

    iterator       begin() noexcept       { return iterator(data.begin(), data.end()); }
    const_iterator begin() const noexcept { return const_iterator(data.begin(), data.end()); }
    iterator       end()   noexcept       { return iterator(data.end(), data.end()); }
    const_iterator end()   const noexcept { return const_iterator(data.end(), data.end()); }
};

}