#define BOOST_TEST_MODULE SAFE_ITERATOR_TEST
#include <boost/test/included/unit_test.hpp>
#include "iterator.hpp"
#include <vector>

BOOST_AUTO_TEST_CASE(safe_iterator_basic) {
    std::vector<int> vec = {1, 2, 3};
    auto it = safe::iterator(vec.begin(), vec.end());    
    
    it->for_each([](const auto& v) { BOOST_CHECK(v == 1); },
                 []{ BOOST_FAIL("Should not call else branch"); });
    ++it;
    it->for_each([](const auto& v) { BOOST_CHECK(v == 2); },
                 []{ BOOST_FAIL("Should not call else branch"); });
    ++it;
    it->for_each([](const auto& v) { BOOST_CHECK(v == 3); },
                 []{ BOOST_FAIL("Should not call else branch"); });
    ++it;
    BOOST_CHECK(!*it); // Should be nullopt at end
    BOOST_CHECK(it == safe::iterator(vec.begin(), vec.end(), vec.end()));
}

BOOST_AUTO_TEST_CASE(safe_iterator_empty_vector) {
    std::vector<int> vec = {};
    auto it = safe::iterator(vec.begin(), vec.end(), vec.begin());
    BOOST_CHECK(!*it); // Should be nullopt at begin
    BOOST_CHECK(it == safe::iterator(vec.begin(), vec.end(), vec.end()));
    ++it; // Still nullopt
    BOOST_CHECK(!*it);
    BOOST_CHECK(it == safe::iterator(vec.begin(), vec.end(), vec.end()));
    --it; // Still nullopt
    BOOST_CHECK(!*it);
    BOOST_CHECK(it == safe::iterator(vec.begin(), vec.end(), vec.end()));
}

BOOST_AUTO_TEST_CASE(safe_iterator_loop) {
    std::vector<int> vec = {1, 2, 3};
    auto it = safe::iterator(vec.begin(), vec.end());
    int sum = 0;
    for(; it != safe::iterator(vec.begin(), vec.end(), vec.end()); ++it)
        it->and_then([&sum](const auto& v) { sum += v; });
    BOOST_CHECK(sum == 6);
}

BOOST_AUTO_TEST_CASE(safe_iterator_out_of_range) {
    std::vector<int> vec = {1, 2, 3};
    
    // Pre begin
    auto it = safe::iterator(vec.begin(), vec.end()); //begin
    --it; // Before begin, will return nullopt
    BOOST_CHECK(!*it);
    BOOST_CHECK(it == safe::iterator(vec.begin(), vec.end()));
    
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

    it--; // Should go back to last
    it->for_each([](const auto& v) { BOOST_CHECK(v == 3); },
                 []{ BOOST_FAIL("Should not call else branch"); });

    // Check if and_then removes refwrap
    bool called = false;
    it->and_then([&called](const auto& v) { BOOST_CHECK(v == 3); called = true; });
    BOOST_CHECK(called);

    it -= 2; // Should go back to first
    it->for_each([](const auto& v) { BOOST_CHECK(v == 1); },
                 []{ BOOST_FAIL("Should not call else branch"); });
    BOOST_CHECK(it == safe::iterator(vec.begin(), vec.end()));
}
