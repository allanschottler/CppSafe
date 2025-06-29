#define BOOST_TEST_MODULE SAFE_ALGORITHM_TEST
#include <boost/test/included/unit_test.hpp>
#include "vector.hpp"
#include "algorithm.hpp"

BOOST_AUTO_TEST_CASE(safe_algorithm_find) {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = safe::find(vec.begin(), vec.end(), 3);
    BOOST_CHECK(it->has_value());
    it->and_then([](const int& val) { BOOST_CHECK(val == 3); });

    auto it_not_found = safe::find(vec.begin(), vec.end(), 6);
    BOOST_CHECK(!it_not_found->has_value());
    bool called = false;
    it_not_found->for_each([](const int& val) { BOOST_FAIL("Should not call then branch"); },
                           [&called]() { called = true; }); // Should call else branch
    BOOST_CHECK(called);
}

BOOST_AUTO_TEST_CASE(safe_algorithm_for_each) {
    safe::vector<int> vec = {1, 2, 3};
    int sum = 0;
    safe::for_each(vec.begin(), vec.end(), [&sum](const int& val) { sum += val; });
    BOOST_CHECK(sum == 6);

    sum = 0;
    safe::for_each(vec, [&sum](const int& val) { sum += val; });
    BOOST_CHECK(sum == 6);
}