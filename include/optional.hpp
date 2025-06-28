#pragma once
#include <optional>

namespace safe {

template<class T>
struct optional {
    optional() = default;
    optional(const T& v) : _value(v) {}
    optional(T&& v) : _value(std::move(v)) {}
    optional(std::nullopt_t) : _value(std::nullopt) {}
    optional(const optional& other) : _value(other._value) {}
    optional(optional&& other) noexcept : _value(std::move(other._value)) 
    { other._value = std::nullopt; } // Clear the moved-from object
    
    optional& operator=(const optional& other) {
        _value = other._value;
        return *this;
    }
    optional& operator=(optional&& other) noexcept {
        _value = std::move(other._value);
        other._value = std::nullopt; // Clear the moved-from object
        return *this;
    }
    
    bool has_value() const noexcept 
    { return _value.has_value(); }
    
    // and_then 
    template<class F, class U = std::invoke_result_t<F, T&>>
    optional<U> and_then(F&& f)
    requires(!std::is_same_v<std::invoke_result_t<F, T&>, void>)
    {
        if (_value.has_value())
            return f(_value.value());
        return std::nullopt;
    }
    
    template<class F>
    void and_then(F&& f) const
    requires(std::is_same_v<std::invoke_result_t<F, T&>, void>)
    {
        if (_value.has_value())
            f(_value.value());
    }

    // for_each
    template<class F, class G>
    void for_each(F&& f, G&& g) const
    requires(std::is_same_v<std::invoke_result_t<F, T&>, void> &&
             std::is_same_v<std::invoke_result_t<G>, void>)
    {
        if (_value.has_value())
            f(_value.value());
        else
            g();
    }
    
    bool operator==(const optional& other) const 
    { return _value == other._value; }
    
    bool operator==(const T& other) const 
    { return _value.has_value() && _value.value() == other; }
    
    bool operator==(std::nullopt_t) const 
    { return !_value.has_value(); }
    
    bool operator!=(const optional& other) const 
    { return !(*this == other); }
    
    bool operator<(const optional& other) const 
    { return _value < other._value; }
    
    bool operator<=(const optional& other) const 
    { return !(other < *this); }
    
    bool operator>(const optional& other) const 
    { return other < *this; }
    
    bool operator>=(const optional& other) const 
    { return !(*this < other); }
    
    explicit operator bool() const noexcept 
    { return _value.has_value(); }
    
private:
    std::optional<T> _value;
};

} // namespace safe