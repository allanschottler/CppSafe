#define BOOST_TEST_MODULE SAFE_ITERATOR_TEST
#include <boost/test/included/unit_test.hpp>
#include "iterator.hpp"
#include <iostream>
#include <vector>

BOOST_AUTO_TEST_CASE(safe_iterator_basic) {
    std::vector<int> vec = {1, 2, 3};
    auto it = safe::iterator(vec.begin(), vec.end(), vec.begin());    
    
    it->for_each([](const auto& v) { BOOST_CHECK(v.get() == 1); },
                 []() { BOOST_FAIL("Should not call else branch"); });
    ++it;
    it->for_each([](const auto& v) { BOOST_CHECK(v.get() == 2); },
                 []() { BOOST_FAIL("Should not call else branch"); });
    ++it;
    it->for_each([](const auto& v) { BOOST_CHECK(v.get() == 3); },
                 []() { BOOST_FAIL("Should not call else branch"); });
    ++it;
    BOOST_CHECK(!*it); // Should be nullopt at end
    BOOST_CHECK(it == safe::iterator(vec.begin(), vec.end(), vec.end()));
}

BOOST_AUTO_TEST_CASE(safe_iterator_out_of_range) {
    std::vector<int> vec = {1, 2, 3};
    
    // Pre begin
    auto it = safe::iterator(vec.begin(), vec.end(), vec.begin()); //begin
    --it; // Before begin, will return nullopt
    BOOST_CHECK(!*it);
    BOOST_CHECK(it == safe::iterator(vec.begin(), vec.end(), vec.begin()));
    
    // Post end
    it = safe::iterator(vec.begin(), vec.end(), vec.end()); //end
    BOOST_CHECK(!*it);
    ++it; // Passed end, will return nullopt
    BOOST_CHECK(!*it);
    BOOST_CHECK(it == safe::iterator(vec.begin(), vec.end(), vec.end()));

    // Post end +
    it++; // Still past end, will return nullopt
    BOOST_CHECK(!*it);
    BOOST_CHECK(it == safe::iterator(vec.begin(), vec.end(), vec.end()));

    --it; // Should go back to last
    it->for_each([](const auto& v) { BOOST_CHECK(v.get() == 3); },
                 []() { BOOST_FAIL("Should not call else branch"); });
}

BOOST_AUTO_TEST_CASE(safe_iterator_loop) {
    std::vector<int> vec = {1, 2, 3};
    auto it = safe::iterator(vec.begin(), vec.end(), vec.begin());
    int sum = 0;
    for(; it != safe::iterator(vec.begin(), vec.end(), vec.end()); ++it)
        it->and_then([&sum](const auto& v) { sum += v.get(); });
    BOOST_CHECK(sum == 6);
}