#pragma once

#include "optional.hpp"
#include <limits>
#include <optional>  // for std::nullopt_t

namespace safe {

template<class T>
requires std::numeric_limits<T>::is_specialized
struct numeric : optional<T> {
    numeric(std::nullopt_t) noexcept : optional<T>(std::nullopt) {}
    numeric() noexcept {}
    numeric(const numeric& o) = default;
    numeric(numeric&& o) noexcept : optional<T>(std::move(o)) {
        o._value = std::nullopt; // Clear the moved-from object
    }

    numeric(unsigned long long v)
    requires(std::is_integral_v<T> && std::is_unsigned_v<T>) {
        if (v < static_cast<unsigned long long>(std::numeric_limits<T>::lowest()) || 
            v > static_cast<unsigned long long>(std::numeric_limits<T>::max()))
            this->_value = std::nullopt;
        else
            this->_value = static_cast<T>(v);
    }
    numeric(long long v)
    requires(std::is_integral_v<T> && std::is_signed_v<T>) {
        if (v < static_cast<long long>(std::numeric_limits<T>::lowest()) || 
            v > static_cast<long long>(std::numeric_limits<T>::max()))
            this->_value = std::nullopt;
        else
            this->_value = static_cast<T>(v);
    }
    numeric(long double v)
    requires(not std::is_integral_v<T>) {
        if (v < static_cast<long double>(std::numeric_limits<T>::lowest()) || 
            v > static_cast<long double>(std::numeric_limits<T>::max()))
            this->_value = std::nullopt;
        else
            this->_value = v;
    }
    numeric& operator=(const numeric& o) {
        this->_value = o._value;
        return *this;
    }
    numeric& operator=(numeric&& o) noexcept {
        this->_value = std::move(o._value);
        o._value = std::nullopt; // Clear the moved-from object
        return *this;
    }
    numeric& operator=(std::nullopt_t) noexcept {
        this->_value = std::nullopt;
        return *this;
    }
    numeric& operator=(long double v)
    requires(not std::is_integral_v<T>) {
        if (v < static_cast<long double>(std::numeric_limits<T>::lowest()) || 
            v > static_cast<long double>(std::numeric_limits<T>::max()))
            this->_value = std::nullopt;
        else
            this->_value = v;
        return *this;
    }
    numeric& operator=(unsigned long long v)
    requires(std::is_integral_v<T> && std::is_unsigned_v<T>) {
        if (v < static_cast<unsigned long long>(std::numeric_limits<T>::lowest()) || 
            v > static_cast<unsigned long long>(std::numeric_limits<T>::max()))
            this->_value = std::nullopt;
        else
            this->_value = static_cast<T>(v);
        return *this;
    }
    numeric& operator=(long long v)
    requires(std::is_integral_v<T> && std::is_signed_v<T>) {
        if (v < static_cast<long long>(std::numeric_limits<T>::lowest()) || 
            v > static_cast<long long>(std::numeric_limits<T>::max()))
            this->_value = std::nullopt;
        else
            this->_value = static_cast<T>(v);
        return *this;
    }

    // T& usafe_get() {
    //     if (!this->_value.has_value())
    //         throw std::runtime_error("Attempted to access value of empty numeric");
    //     return this->_value.value();
    // }

    // const T& usafe_get() const {
    //     if (!this->_value.has_value())
    //         throw std::runtime_error("Attempted to access value of empty numeric");
    //     return this->_value.value();
    // }

    // T& operator*() {
    //     return usafe_get();
    // }
    // const T& operator*() const {
    //     return usafe_get();
    // }

    // addition
    numeric operator+(const numeric& o) const {
        if (!this->_value.has_value() || !o._value.has_value())
            return numeric(std::nullopt);
        T a = this->_value.value(), b = o._value.value();
        if (a > std::numeric_limits<T>::max() - b)
            return numeric(std::nullopt);
        return numeric(a + b);
    }
    numeric operator+(T v) const {
        if (!this->_value.has_value())
            return numeric(std::nullopt);
        T a = this->_value.value();
        if (a > std::numeric_limits<T>::max() - v)
            return numeric(std::nullopt);
        return numeric(a + v);
    }
    numeric& operator+=(const numeric& o) {
        if (!this->_value.has_value() || !o._value.has_value())
            return *this = numeric(std::nullopt);
        T a = this->_value.value(), b = o._value.value();
        if (a > std::numeric_limits<T>::max() - b)
            return *this = numeric(std::nullopt);
        this->_value = a + b;
        return *this;
    }
    numeric& operator+=(T v) {
        if (!this->_value.has_value())
            return *this = numeric(std::nullopt);
        T a = this->_value.value();
        if (a > std::numeric_limits<T>::max() - v)
            return *this = numeric(std::nullopt);
        this->_value = a + v;
        return *this;
    }

    // subtraction
    numeric operator-(const numeric& o) const {
        if (!this->_value.has_value() || !o._value.has_value())
            return numeric(std::nullopt);
        T a = this->_value.value(), b = o._value.value();
        if (a < std::numeric_limits<T>::lowest() + b)
            return numeric(std::nullopt);
        return numeric(a - b);
    }
    numeric operator-(T v) const {
        if (!this->_value.has_value())
            return numeric(std::nullopt);
        T a = this->_value.value();
        if (a < std::numeric_limits<T>::lowest() + v)
            return numeric(std::nullopt);
        return numeric(a - v);
    }
    numeric& operator-=(const numeric& o) {
        if (!this->_value.has_value() || !o._value.has_value())
            return *this = numeric(std::nullopt);
        T a = this->_value.value(), b = o._value.value();
        if (a < std::numeric_limits<T>::lowest() + b)
            return *this = numeric(std::nullopt);
        this->_value = a - b;
        return *this;
    }
    numeric& operator-=(T v) {
        if (!this->_value.has_value())
            return *this = numeric(std::nullopt);
        T a = this->_value.value();
        if (a < std::numeric_limits<T>::lowest() + v)
            return *this = numeric(std::nullopt);
        this->_value = a - v;
        return *this;
    }

    // multiplication
    numeric operator*(const numeric& o) const {
        if (!this->_value.has_value() || !o._value.has_value())
            return numeric(std::nullopt);
        T a = this->_value.value(), b = o._value.value();
        if (a > std::numeric_limits<T>::max() / b)
            return numeric(std::nullopt);
        return numeric(a * b);
    }
    numeric operator*(T v) const {
        if (!this->_value.has_value())
            return numeric(std::nullopt);
        T a = this->_value.value();
        if (a > std::numeric_limits<T>::max() / v)
            return numeric(std::nullopt);
        return numeric(a * v);
    }

    // division
    numeric operator/(const numeric& o) const {
        if (!this->_value.has_value() || !o._value.has_value() || o._value.value() == T(0))
            return numeric(std::nullopt);
        return numeric(this->_value.value() / o._value.value());
    }
    numeric operator/(T v) const {
        if (!this->_value.has_value() || v == T(0))
            return numeric(std::nullopt);
        return numeric(this->_value.value() / v);
    }

    // modulus
    numeric operator%(const numeric& o) const {
        if (!this->_value.has_value() || !o._value.has_value() || o._value.value() == T(0))
            return numeric(std::nullopt);
        return numeric(this->_value.value() % o._value.value());
    }
    numeric operator%(T v) const {
        if (!this->_value.has_value() || v == T(0))
            return numeric(std::nullopt);
        return numeric(this->_value.value() % v);
    }

    // unary negation
    numeric operator-() const {
        if constexpr (!std::numeric_limits<T>::is_signed)
            return numeric(std::nullopt);
        if (!this->_value.has_value())
            return numeric(std::nullopt);
        return numeric(- this->_value.value());
    }

    // increment/decrement
    numeric& operator++() {
        if (!this->_value.has_value() || this->_value.value() == std::numeric_limits<T>::max())
            return *this = numeric(std::nullopt);
        this->_value = this->_value.value() + T(1);
        return *this;
    }
    numeric operator++(int) {
        numeric tmp = *this;
        ++*this;
        return tmp;
    }
    numeric& operator--() {
        if constexpr (!std::numeric_limits<T>::is_signed) {
            return *this = numeric(std::nullopt);
        }
        if (!this->_value.has_value() || this->_value.value() == std::numeric_limits<T>::lowest())
            return *this = numeric(std::nullopt);
        this->_value = this->_value.value() - T(1);
        return *this;
    }
    numeric operator--(int) {
        numeric tmp = *this;
        --*this;
        return tmp;
    }
};

// literal suffixes
constexpr numeric<int>                operator"" _si (unsigned long long v) noexcept { return numeric<int>(v); }
constexpr numeric<unsigned int>       operator"" _su (unsigned long long v) noexcept { return numeric<unsigned int>(v); }
constexpr numeric<long>               operator"" _sl (unsigned long long v) noexcept { return numeric<long>(v); }
constexpr numeric<unsigned long>      operator"" _sul(unsigned long long v) noexcept { return numeric<unsigned long>(v); }
constexpr numeric<long long>          operator"" _sll(unsigned long long v) noexcept { return numeric<long long>(v); }
constexpr numeric<unsigned long long> operator"" _sull(unsigned long long v) noexcept { return numeric<unsigned long long>(v); }
constexpr numeric<char>               operator"" _sc (unsigned long long v) noexcept { return numeric<char>(v); }
constexpr numeric<unsigned char>      operator"" _suc(unsigned long long v) noexcept { return numeric<unsigned char>(v); }
constexpr numeric<short>              operator"" _ss (unsigned long long v) noexcept { return numeric<short>(v); }
constexpr numeric<unsigned short>     operator"" _sus(unsigned long long v) noexcept { return numeric<unsigned short>(v); }
constexpr numeric<float>              operator"" _sf (long double v) noexcept       { return numeric<float>(v); }
constexpr numeric<double>             operator"" _sd (long double v) noexcept       { return numeric<double>(v); }
constexpr numeric<long double>        operator"" _sld(long double v) noexcept       { return numeric<long double>(v); }

} // namespace safe
