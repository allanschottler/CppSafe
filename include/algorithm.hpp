#pragma once

#include <optional>
#include <algorithm>

namespace safe {

// find: returns std::optional<SafeIter> that holds the iterator to the
// first element equal to value, or std::nullopt if not found.
template<typename SafeIter, typename T>
std::optional<SafeIter> find(SafeIter first, SafeIter last, const T& value) {
    auto it = std::find(first, last, value);
    if (it != last) {
        return it;
    }
    return std::nullopt;
}

// find_if: returns std::optional<SafeIter> to the first element
// satisfying the predicate, or std::nullopt if none does.
template<typename SafeIter, typename Pred>
std::optional<SafeIter> find_if(SafeIter first, SafeIter last, Pred predicate) {
    auto it = std::find_if(first, last, predicate);
    if (it != last) {
        return it;
    }
    return std::nullopt;
}

// lower_bound: returns std::optional<SafeIter> to the first element
// not less than value, or std::nullopt if all elements compare less.
template<typename SafeIter, typename T>
std::optional<SafeIter> lower_bound(SafeIter first, SafeIter last, const T& value) {
    auto it = std::lower_bound(first, last, value);
    if (it != last) {
        return it;
    }
    return std::nullopt;
}

} // namespace safe