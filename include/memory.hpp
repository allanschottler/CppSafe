#pragma once
#include "optional.hpp"
#include <memory>

namespace safe {

template<class T>
struct pointer {
    using element_type = T;
    using deref_type = optional<std::reference_wrapper<element_type>>;

    pointer() : ptr_(nullptr) {}
    pointer(pointer&& other) noexcept : ptr_(std::move(other.ptr_)) 
    { other.ptr_ = nullptr; }
    pointer(T*&& ptr) : ptr_(std::move(ptr)) 
    { ptr = nullptr; }
    
    pointer(const pointer&) = delete;
    pointer& operator=(const pointer&) = delete;
    pointer& operator=(T* ptr) = delete;

    pointer& operator=(pointer&& other) noexcept {
        reset();
        ptr_ = std::move(other.ptr_);
        other.ptr_ = nullptr; // Clear the moved-from object
        return *this;
    }

    pointer& operator=(T*&& ptr) {
        reset();
        ptr_ = std::move(ptr);
        ptr = nullptr; // Nullify the original pointer to avoid double deletion
        return *this;
    }

    pointer& operator=(std::nullptr_t) {
        reset();
        return *this;
    }    

    deref_type operator*() const { 
        if (ptr_)
            return std::ref(*ptr_);
        return std::nullopt;
    }

    struct arrow_proxy {
        deref_type opt;
        deref_type* operator->() { return &opt; }
    };

    arrow_proxy operator->() {
        arrow_proxy p;
        if (ptr_)
            p.opt = std::ref(*ptr_);
        return p;
    }

    void for_each(auto&& then_branch, auto&& else_branch) const {
        if (ptr_)
            then_branch(*ptr_);
        else
            else_branch();
    }

    void reset() noexcept {
        if (ptr_) {
            delete ptr_;
            ptr_ = nullptr;
        }
    }

    explicit operator bool() const noexcept 
    { return ptr_ != nullptr; }

private:
    T* ptr_;
};

template<class T>
struct unique_ptr {
    using element_type = T;
    using pointer_type = std::unique_ptr<T>;
    using deref_type = optional<std::reference_wrapper<element_type>>;

    unique_ptr() : ptr_(nullptr) {}
    unique_ptr(T*&& ptr) : ptr_(ptr) {}
    unique_ptr(pointer_type&& ptr) : ptr_(std::move(ptr)) {}

    deref_type operator*() const {
        if (ptr_)
            return std::ref(*ptr_);
        return std::nullopt;
    }

    struct arrow_proxy {
        deref_type opt;
        deref_type* operator->() { return &opt; }
    };

    arrow_proxy operator->() {
        arrow_proxy p;
        if (ptr_)
            p.opt = std::ref(*ptr_);
        return p;
    }

    void reset() noexcept { ptr_.reset(); }

    explicit operator bool() const noexcept 
    { return static_cast<bool>(ptr_); }

    // Move semantics
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr(unique_ptr&& other) noexcept : ptr_(std::move(other.ptr_)) {
        other.ptr_ = nullptr; // Clear the moved-from object
    }
    unique_ptr& operator=(const unique_ptr&) = delete;
    unique_ptr& operator=(unique_ptr&& other) noexcept {
        if (this != &other) {
            ptr_ = std::move(other.ptr_);
            other.ptr_ = nullptr; // Clear the moved-from object
        }
        return *this;
    }

private:
    pointer_type ptr_;
};

} // namespace safe