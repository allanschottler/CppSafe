#define BOOST_TEST_MODULE SAFE_ITERATOR_TEST
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <optional>
#include "iterator.hpp"

BOOST_AUTO_TEST_CASE(safe_iterator_basic) {
    std::vector<int> vec = {1, 2, 3};
    auto it = safe::iterator(vec.begin(), vec.end(), vec.begin());
    BOOST_CHECK(*it == std::optional<int>(1));
    ++it;
    BOOST_CHECK(*it == std::optional<int>(2));
    ++it;
    BOOST_CHECK(*it == std::optional<int>(3));
    ++it;
    BOOST_CHECK(!*it);
    BOOST_CHECK(it == safe::iterator(vec.begin(), vec.end(), vec.end()));
    ++it;
    BOOST_CHECK(it == safe::iterator(vec.begin(), vec.end(), vec.end()));
    --it;
    BOOST_CHECK(*it == std::optional<int>(3));
    --it;
    BOOST_CHECK(*it == std::optional<int>(2));
    --it;
    BOOST_CHECK(*it == std::optional<int>(1));
    --it;
    BOOST_CHECK(it == safe::iterator(vec.begin(), vec.end(), vec.begin()));
    BOOST_CHECK(std::next(it,10) == safe::iterator(vec.begin(), vec.end(), vec.end()));
    // it = safe::iterator(vec.begin(), vec.end(), vec.begin());
    // it += 2;
    // BOOST_CHECK(*it == std::optional<int>(3));
    // it += 2;
    // BOOST_CHECK(*it == std::nullopt);
    // it -= 1;
    // BOOST_CHECK(*it == std::optional<int>(3));
    // it -= 20;
    // BOOST_CHECK(*it == std::nullopt);
}

BOOST_AUTO_TEST_CASE(safe_iterator_loop) {
    std::vector<int> vec = {1, 2, 3};
    auto it = safe::iterator(vec.begin(), vec.end(), vec.begin());
    int sum = 0;
    for (; it != safe::iterator(vec.begin(), vec.end(), vec.end()); ++it) {
        if (auto val = *it) {
            sum += val.value();
        }
    }
    BOOST_CHECK(sum == 6);
}