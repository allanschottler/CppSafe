# CppSafe

Header-only C++23 experiment in making the standard library *safe by construction*: no undefined behavior from out-of-bounds access, dangling iterators, null pointers or integer overflow. Every potentially failing access returns a `safe::optional`, and values are consumed through callbacks instead of being dereferenced directly.

## What's inside

| Header | Provides |
|---|---|
| `optional.hpp` | `safe::optional<T>` with no unchecked `operator*`; use `and_then` (chainable) or `for_each(then, else)` |
| `vector.hpp` | `safe::vector<T>`: `at`/`operator[]` return optionals, allocation failures don't throw |
| `iterator.hpp` | `safe::iterator<It>`: bounds-aware, dereferences to an optional, clamps instead of running past `begin`/`end` |
| `memory.hpp` | `safe::pointer<T>`, `safe::unique_ptr<T>`: null-safe access, moved-from objects are emptied |
| `numeric.hpp` | `safe::numeric<T>`: overflow, underflow, division by zero, bad conversions and bad string parses yield empty values; literals like `10_si`, `3_su`, `255_suc`, `1.5_sf` |
| `algorithm.hpp` | `safe::find`, `find_if`, `find_if_not`, `lower_bound`, `for_each` returning safe iterators |

## Example

```cpp
#include "vector.hpp"
#include "numeric.hpp"

safe::vector<int> v{1, 2, 3};
v[5].for_each([](const int& x) { /* in range */ },
              [] { /* out of bounds, no UB */ });

using safe::operator""_si;
auto big = safe::numeric<int>::max();
bool overflowed = (big + 1_si) == std::nullopt; // true
bool divByZero = (1_si / 0) == std::nullopt; // true
auto parsed = safe::numeric<int>("42"); // 42
auto bad = safe::numeric<unsigned>("-12"); // empty
```

## Build tests and benchmarks

Requires a C++23 compiler, CMake ≥ 3.16 and [Conan 2](https://conan.io) (pulls Boost.Test and Google Benchmark).

```sh
conan install . --build=missing
cmake --preset conan-release
cmake --build --preset conan-release
ctest --preset conan-release
```

To use the library, just add `include/` to your include path.

## License

MIT
