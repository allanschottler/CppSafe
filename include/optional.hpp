#pragma once
#include <optional>

namespace safe {
    template<class T>
    class optional {
        std::optional<T> _value;
    public:
        optional() = default;
        optional(const T& v) : _value(v) {}
        optional(T&& v) : _value(std::move(v)) {}
        optional(std::nullopt_t) : _value(std::nullopt) {}
        optional(const optional& other) : _value(other._value) {}
        optional(optional&& other) noexcept : _value(std::move(other._value)) {}

        optional& operator=(const optional& other) {
            if (this != &other) {
                _value = other._value;
            }
            return *this;
        }
        optional& operator=(optional&& other) noexcept {
            if (this != &other) {
                _value = std::move(other._value);
            }
            return *this;
        }

        T& value() {
            if (!_value.has_value()) {
                throw std::bad_optional_access();
            }
            return _value.value();
        }
        bool has_value() const noexcept { return _value.has_value(); }
        void reset() { _value.reset(); }

        template<class F, class U = std::invoke_result_t<F, T&>>
        optional<U> and_then(F&& f)
            requires(!std::is_same_v<std::invoke_result_t<F, T&>, void>)
        {
            if (_value.has_value()) {
                return f(_value.value());
            }
            return std::nullopt;
        }

        template<class F>
        void and_then(F&& f) const
            requires(std::is_same_v<std::invoke_result_t<F, T&>, void>)
        {
            if (_value.has_value()) {
                f(_value.value());
            }
        }

        bool operator==(const optional& other) const {
            return _value == other._value;
        }
        bool operator==(const T& other) const {
            return _value.has_value() && _value.value() == other;
        }
        bool operator==(std::nullopt_t) const {
            return !_value.has_value();
        }
        bool operator!=(const optional& other) const {
            return !(*this == other);
        }
        bool operator<(const optional& other) const {
            return _value < other._value;
        }
        bool operator<=(const optional& other) const {
            return !(other < *this);
        }
        bool operator>(const optional& other) const {
            return other < *this;
        }
        bool operator>=(const optional& other) const {
            return !(*this < other);
        }

        T& value_or(const T& default_value) {
            return _value.value_or(default_value);
        }
        const T& value_or(const T& default_value) const {
            return _value.value_or(default_value);
        }

        T& operator*() { return _value.value(); }
        const T& operator*() const { return _value.value(); }

        T* operator->() { return _value.has_value() ? &_value.value() : nullptr; }
        const T* operator->() const { return _value.has_value() ? &_value.value() : nullptr; }

        explicit operator bool() const noexcept { return _value.has_value(); }

        void operator=(const T& v) { _value = v; }
        void operator=(T&& v) { _value = std::move(v); }
        void operator=(std::nullopt_t) { _value.reset(); }
    };
}