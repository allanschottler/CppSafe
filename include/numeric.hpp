#pragma once
#include "optional.hpp"
#include <limits>

namespace safe {

template<class T>
struct numeric
{
    T value;

    numeric() = default;
    numeric(const T& v) : value(v) {}
    numeric(T&& v) : value(std::move(v)) {}
    numeric(const numeric& other) : value(other.value) {}
    numeric(numeric&& other) noexcept : value(std::move(other.value)) {}

    numeric& operator=(const T& v) {
        value = v;
        return *this;
    }
    numeric& operator=(T&& v) {
        value = std::move(v);
        return *this;
    }
    numeric& operator=(const numeric& other) {
        value = other.value;
        return *this;
    }
    numeric& operator=(numeric&& other) noexcept {
        value = std::move(other.value);
        return *this;
    }

    const T& get() const 
    { return value; }

    T& get() 
    { return value; }

    const T& operator*() const 
    { return value; }

    T& operator*() 
    { return value; }

    bool operator==(const numeric& other) const 
    { return value == other.value; }

    bool operator!=(const numeric& other) const 
    { return !(*this == other); }

    bool operator<(const numeric& other) const 
    { return value < other.value; }

    bool operator<=(const numeric& other) const 
    { return value <= other.value; }

    bool operator>(const numeric& other) const 
    { return value > other.value; }

    bool operator>=(const numeric& other) const 
    { return value >= other.value; }

    optional<numeric> operator+(const numeric& other) const {
        if(value > std::numeric_limits<T>::max() - other.value)
            return std::nullopt; // Overflow
        return numeric(value + other.value);
    }

    optional<numeric> operator-(const numeric& other) const {
        if(value < std::numeric_limits<T>::min() + other.value)
            return std::nullopt; // Underflow
        return numeric(value - other.value);
    }

    optional<numeric> operator*(const numeric& other) const {    
        if(value > std::numeric_limits<T>::max() / other.value)
            return std::nullopt; // Overflow    
        return numeric(value * other.value);
    }

    optional<numeric> operator/(const numeric& other) const {
        if (*other == 0)
            return std::nullopt; // Division by zero
        return numeric(value / other.value);
    }
};

} // namespace safe