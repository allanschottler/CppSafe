#pragma once
#include "optional.hpp"
#include <limits>
#include <optional>
#include <functional>

namespace safe {

template<class T>
requires std::numeric_limits<T>::is_specialized
struct numeric;

template<class T>
requires std::numeric_limits<T>::is_specialized
auto from_string(std::string_view v) -> numeric<T>;

template<class T>
requires std::numeric_limits<T>::is_specialized
struct numeric {
    numeric(std::nullopt_t) noexcept : _value(std::nullopt) {}
    numeric() noexcept = default;
    numeric(const numeric& o) = default;
    numeric(numeric&& o) noexcept : _value(std::move(o._value)) { o._value = std::nullopt; }
    numeric(const optional<T>& o) : _value(o) {}
    numeric(optional<T>&& o) noexcept : _value(std::move(o)) {}
    
    numeric(unsigned long long v)
    requires(std::is_integral_v<T> && std::is_unsigned_v<T>) {
        if (v < static_cast<unsigned long long>(std::numeric_limits<T>::lowest()) ||
            v > static_cast<unsigned long long>(std::numeric_limits<T>::max()))
            _value = std::nullopt;
        else
            _value = static_cast<T>(v);
    }
    numeric(long long v)
    requires(std::is_integral_v<T> && std::is_signed_v<T>) {
        if (v < static_cast<long long>(std::numeric_limits<T>::lowest()) ||
            v > static_cast<long long>(std::numeric_limits<T>::max()))
            _value = std::nullopt;
        else
            _value = static_cast<T>(v);
    }
    numeric(long double v)
    requires(not std::is_integral_v<T>) {
        if (std::isnan(v) ||
            v < static_cast<long double>(std::numeric_limits<T>::lowest()) ||
            v > static_cast<long double>(std::numeric_limits<T>::max()))
            _value = std::nullopt;
        else
            _value = v;
    }
    
    explicit numeric(std::string_view v) : numeric(from_string<T>(v)) {}

    numeric& operator=(const numeric& o) {
        _value = o._value;
        return *this;
    }
    numeric& operator=(numeric&& o) noexcept {
        _value = std::move(o._value);
        o._value = std::nullopt;
        return *this;
    }
    numeric& operator=(std::nullopt_t) noexcept {
        _value = std::nullopt;
        return *this;
    }
    numeric& operator=(long double v)
    requires(not std::is_integral_v<T>) {
        if (v < static_cast<long double>(std::numeric_limits<T>::lowest()) ||
            v > static_cast<long double>(std::numeric_limits<T>::max()))
            _value = std::nullopt;
        else
            _value = v;
        return *this;
    }
    numeric& operator=(unsigned long long v)
    requires(std::is_integral_v<T> && std::is_unsigned_v<T>) {
        if (v < static_cast<unsigned long long>(std::numeric_limits<T>::lowest()) ||
            v > static_cast<unsigned long long>(std::numeric_limits<T>::max()))
            _value = std::nullopt;
        else
            _value = static_cast<T>(v);
        return *this;
    }
    numeric& operator=(long long v)
    requires(std::is_integral_v<T> && std::is_signed_v<T>) {
        if (v < static_cast<long long>(std::numeric_limits<T>::lowest()) ||
            v > static_cast<long long>(std::numeric_limits<T>::max()))
            _value = std::nullopt;
        else
            _value = static_cast<T>(v);
        return *this;
    }

    bool operator==(const numeric& o) const 
    { return _value == o._value; }
    bool operator!=(const numeric& o) const 
    { return !(*this == o); }
    bool operator<(const numeric& o) const {
        if (!_value.has_value() && o._value.has_value()) return true;
        if (_value.has_value() && !o._value.has_value()) return false;
        if (!_value.has_value() && !o._value.has_value()) return false;
        return unsafe_ref_get<numeric>{}(*this) < unsafe_ref_get<numeric>{}(o);
    }    
    bool operator<=(const numeric& o) const 
    { return !(o < *this); }
    bool operator>(const numeric& o) const 
    { return o < *this; }
    bool operator>=(const numeric& o) const 
    { return !(*this < o); }

    numeric operator+(const numeric& o) const {
        if (!_value.has_value() || !o._value.has_value())
            return numeric(std::nullopt);
        T a = unsafe_ref_get<numeric>{}(_value), b = unsafe_ref_get<numeric>{}(o._value);
        if (a > std::numeric_limits<T>::max() - b)
            return numeric(std::nullopt);
        return numeric(a + b);
    }
    numeric operator+(T v) const {
        if (!_value.has_value())
            return numeric(std::nullopt);
        T a = unsafe_ref_get<numeric>{}(_value);
        if (a > std::numeric_limits<T>::max() - v)
            return numeric(std::nullopt);
        return numeric(a + v);
    }
    numeric& operator+=(const numeric& o) {
        if (!_value.has_value() || !o._value.has_value())
            return *this = numeric(std::nullopt);
        T a = unsafe_ref_get<numeric>{}(_value), b = unsafe_ref_get<numeric>{}(o._value);
        if (a > std::numeric_limits<T>::max() - b)
            return *this = numeric(std::nullopt);
        _value = a + b;
        return *this;
    }
    numeric& operator+=(T v) {
        if (!_value.has_value())
            return *this = numeric(std::nullopt);
        T a = unsafe_ref_get<numeric>{}(_value);
        if (a > std::numeric_limits<T>::max() - v)
            return *this = numeric(std::nullopt);
        _value = a + v;
        return *this;
    }

    numeric operator-(const numeric& o) const {
        if (!_value.has_value() || !o._value.has_value())
            return numeric(std::nullopt);
        T a = unsafe_ref_get<numeric>{}(_value), b = unsafe_ref_get<numeric>{}(o._value);
        if (a < std::numeric_limits<T>::lowest() + b)
            return numeric(std::nullopt);
        return numeric(a - b);
    }
    numeric operator-(T v) const {
        if (!_value.has_value())
            return numeric(std::nullopt);
        T a = unsafe_ref_get<numeric>{}(_value);
        if (a < std::numeric_limits<T>::lowest() + v)
            return numeric(std::nullopt);
        return numeric(a - v);
    }
    numeric& operator-=(const numeric& o) {
        if (!_value.has_value() || !o._value.has_value())
            return *this = numeric(std::nullopt);
        T a = unsafe_ref_get<numeric>{}(_value), b = unsafe_ref_get<numeric>{}(o._value);
        if (a < std::numeric_limits<T>::lowest() + b)
            return *this = numeric(std::nullopt);
        _value = a - b;
        return *this;
    }
    numeric& operator-=(T v) {
        if (!_value.has_value())
            return *this = numeric(std::nullopt);
        T a = unsafe_ref_get<numeric>{}(_value);
        if (a < std::numeric_limits<T>::lowest() + v)
            return *this = numeric(std::nullopt);
        _value = a - v;
        return *this;
    }

    numeric operator*(const numeric& o) const {
        if (!_value.has_value() || !o._value.has_value())
            return numeric(std::nullopt);
        T a = unsafe_ref_get<numeric>{}(_value), b = unsafe_ref_get<numeric>{}(o._value);
        if (a > std::numeric_limits<T>::max() / b)
            return numeric(std::nullopt);
        return numeric(a * b);
    }
    numeric operator*(T v) const {
        if (!_value.has_value())
            return numeric(std::nullopt);
        T a = unsafe_ref_get<numeric>{}(_value);
        if (a > std::numeric_limits<T>::max() / v)
            return numeric(std::nullopt);
        return numeric(a * v);
    }

    numeric operator/(const numeric& o) const {
        if (!_value.has_value() || !o._value.has_value() || unsafe_ref_get<numeric>{}(o._value) == T(0))
            return numeric(std::nullopt);
        return numeric(unsafe_ref_get<numeric>{}(_value) / unsafe_ref_get<numeric>{}(o._value));
    }
    numeric operator/(T v) const {
        if (!_value.has_value() || v == T(0))
            return numeric(std::nullopt);
        return numeric(unsafe_ref_get<numeric>{}(_value) / v);
    }

    numeric operator%(const numeric& o) const {
        if (!_value.has_value() || !o._value.has_value() || unsafe_ref_get<numeric>{}(o._value) == T(0))
            return numeric(std::nullopt);
        return numeric(unsafe_ref_get<numeric>{}(_value) % unsafe_ref_get<numeric>{}(o._value));
    }
    numeric operator%(T v) const {
        if (!_value.has_value() || v == T(0))
            return numeric(std::nullopt);
        return numeric(unsafe_ref_get<numeric>{}(_value) % v);
    }

    numeric operator-() const {
        if constexpr (!std::numeric_limits<T>::is_signed)
            return numeric(std::nullopt);
        if (!_value.has_value())
            return numeric(std::nullopt);
        return numeric(-unsafe_ref_get<numeric>{}(_value));
    }

    numeric& operator++() {
        if (!_value.has_value() || unsafe_ref_get<numeric>{}(_value) == std::numeric_limits<T>::max())
            return *this = numeric(std::nullopt);
        _value = unsafe_ref_get<numeric>{}(_value) + T(1);
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
        if (!_value.has_value() || unsafe_ref_get<numeric>{}(_value) == std::numeric_limits<T>::lowest())
            return *this = numeric(std::nullopt);
        _value = unsafe_ref_get<numeric>{}(_value) - T(1);
        return *this;
    }
    numeric operator--(int) {
        numeric tmp = *this;
        --*this;
        return tmp;
    }

    static constexpr numeric min() noexcept 
    { return numeric(std::numeric_limits<T>::min()); }
    static constexpr numeric max() noexcept 
    { return numeric(std::numeric_limits<T>::max()); }
    static constexpr numeric lowest() noexcept 
    { return numeric(std::numeric_limits<T>::lowest()); }
    static constexpr numeric epsilon() noexcept 
    { return numeric(std::numeric_limits<T>::epsilon()); }

    bool has_value() const noexcept 
    { return _value.has_value(); }
    explicit operator bool() const noexcept 
    { return _value.has_value(); }

    void and_then(auto&& f) const
    { _value.and_then(std::forward<decltype(f)>(f)); }
    void for_each(auto&& f, auto&& g) const 
    { _value.for_each(std::forward<decltype(f)>(f), std::forward<decltype(g)>(g)); }

private:
    friend unsafe_ref_get<numeric>;
    optional<T> _value;
};

// unsafe_ref_get specialization for numeric
template<class T>
struct unsafe_ref_get<numeric<T>> {
    T& operator()(numeric<T>& n) const {
        return unsafe_ref_get<optional<T>>{}(n._value);
    }
    const T& operator()(const numeric<T>& n) const {
        return unsafe_ref_get<optional<T>>{}(n._value);
    }
};

// comparisons
template<class T, std::convertible_to<T> U>
requires std::numeric_limits<T>::is_specialized
inline bool operator==(const U& lhs, const numeric<T>& rhs) { return rhs == static_cast<T>(lhs); }
template<class T, std::convertible_to<T> U>
requires std::numeric_limits<T>::is_specialized
inline bool operator!=(const U& lhs, const numeric<T>& rhs) { return !(lhs == rhs); }
template<class T, std::convertible_to<T> U>
requires std::numeric_limits<T>::is_specialized
inline bool operator<(const U& lhs, const numeric<T>& rhs)  { return static_cast<T>(lhs) < rhs; }
template<class T, std::convertible_to<T> U>
requires std::numeric_limits<T>::is_specialized
inline bool operator<=(const U& lhs, const numeric<T>& rhs) { return !(rhs < lhs); }
template<class T, std::convertible_to<T> U>
requires std::numeric_limits<T>::is_specialized
inline bool operator>(const U& lhs, const numeric<T>& rhs)  { return rhs < lhs; }
template<class T, std::convertible_to<T> U>
requires std::numeric_limits<T>::is_specialized
inline bool operator>=(const U& lhs, const numeric<T>& rhs) { return !(lhs < rhs); }

// arithmetic
template<class T, std::convertible_to<T> U>
requires std::numeric_limits<T>::is_specialized
inline numeric<T> operator+(const U& lhs, const numeric<T>& rhs) {
    if (!rhs.has_value()) return numeric<T>(std::nullopt);
    T l = static_cast<T>(lhs), r = unsafe_ref_get<numeric<T>>{}(rhs);
    if (l > std::numeric_limits<T>::max() - r) return numeric<T>(std::nullopt);
    return numeric<T>(l + r);
}
template<class T, std::convertible_to<T> U>
requires std::numeric_limits<T>::is_specialized
inline numeric<T> operator-(const U& lhs, const numeric<T>& rhs) {
    if (!rhs.has_value()) return numeric<T>(std::nullopt);
    T l = static_cast<T>(lhs), r = unsafe_ref_get<numeric<T>>{}(rhs);
    if (l < std::numeric_limits<T>::lowest() + r) return numeric<T>(std::nullopt);
    return numeric<T>(l - r);
}
template<class T, std::convertible_to<T> U>
requires std::numeric_limits<T>::is_specialized
inline numeric<T> operator*(const U& lhs, const numeric<T>& rhs) {
    if (!rhs.has_value()) return numeric<T>(std::nullopt);
    T l = static_cast<T>(lhs), r = unsafe_ref_get<numeric<T>>{}(rhs);
    if (l > std::numeric_limits<T>::max() / r) return numeric<T>(std::nullopt);
    return numeric<T>(l * r);
}
template<class T, std::convertible_to<T> U>
requires std::numeric_limits<T>::is_specialized
inline numeric<T> operator/(const U& lhs, const numeric<T>& rhs) {
    if (!rhs.has_value() || unsafe_ref_get<numeric<T>>{}(rhs) == T(0)) return numeric<T>(std::nullopt);
    T l = static_cast<T>(lhs), r = unsafe_ref_get<numeric<T>>{}(rhs);
    return numeric<T>(l / r);
}
template<class T, std::convertible_to<T> U>
requires std::numeric_limits<T>::is_specialized
inline numeric<T> operator%(const U& lhs, const numeric<T>& rhs) {
    if (!rhs._value.has_value() || unsafe_ref_get<numeric<T>>{}(rhs) == T(0)) return numeric<T>(std::nullopt);
    T l = static_cast<T>(lhs), r = unsafe_ref_get<numeric<T>>{}(rhs);
    return numeric<T>(l % r);
}

constexpr numeric<int>                operator"" _si  (unsigned long long v) noexcept { return numeric<int>(v); }
constexpr numeric<unsigned int>       operator"" _su  (unsigned long long v) noexcept { return numeric<unsigned int>(v); }
constexpr numeric<long>               operator"" _sl  (unsigned long long v) noexcept { return numeric<long>(v); }
constexpr numeric<unsigned long>      operator"" _sul (unsigned long long v) noexcept { return numeric<unsigned long>(v); }
constexpr numeric<long long>          operator"" _sll (unsigned long long v) noexcept { return numeric<long long>(v); }
constexpr numeric<unsigned long long> operator"" _sull(unsigned long long v) noexcept { return numeric<unsigned long long>(v); }
constexpr numeric<char>               operator"" _sc  (unsigned long long v) noexcept { return numeric<char>(v); }
constexpr numeric<unsigned char>      operator"" _suc (unsigned long long v) noexcept { return numeric<unsigned char>(v); }
constexpr numeric<short>              operator"" _ss  (unsigned long long v) noexcept { return numeric<short>(v); }
constexpr numeric<unsigned short>     operator"" _sus (unsigned long long v) noexcept { return numeric<unsigned short>(v); }
constexpr numeric<float>              operator"" _sf  (long double v)        noexcept { return numeric<float>(v); }
constexpr numeric<double>             operator"" _sd  (long double v)        noexcept { return numeric<double>(v); }
constexpr numeric<long double>        operator"" _sld (long double v)        noexcept { return numeric<long double>(v); }

template<class T>
requires std::numeric_limits<T>::is_specialized
auto from_string(std::string_view v) -> numeric<T> {
    try {
        if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>) {
            if (!v.empty() && v.front() == '-') {
                return std::nullopt;
            } else {
                return numeric<T>(std::stoull(std::string(v)));
            }
        } else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
            return numeric<T>(std::stoll(std::string(v)));
        } else if constexpr (std::is_floating_point_v<T>) {
            return numeric<T>(std::stod(std::string(v)));
        } else {
            return std::nullopt;
        }
    } catch (...) {
        return std::nullopt;
    }
}

} // namespace safe