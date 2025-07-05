#define BOOST_TEST_MODULE SAFE_VECTOR_TEST
#include <boost/test/included/unit_test.hpp>
#include "vector.hpp"

BOOST_AUTO_TEST_CASE(safe_vector_basic) {
    safe::vector<int> vec{1, 2, 3};   

    auto opt_val = vec.at(1);
    BOOST_CHECK(opt_val.has_value());
    opt_val.for_each([](const int& v) { BOOST_CHECK(v == 2); },
                     []{ BOOST_FAIL("Should not call else branch"); });

    opt_val = vec.at(5);
    BOOST_CHECK(!opt_val.has_value());
}

BOOST_AUTO_TEST_CASE(safe_vector_loop) {
    safe::vector<int> vec{1, 2, 3};  

    int sum = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it)
        if (auto val = *it)
            val.and_then([&sum](const int& v) { sum += v; });
    BOOST_CHECK(sum == 6);

    sum = 0;
    for(const auto& val : vec)
        val.and_then([&sum](const int& v) { sum += v; });
    BOOST_CHECK(sum == 6);
}

BOOST_AUTO_TEST_CASE(safe_vector_alloc) {
    safe::vector<int> vec(1000000000000); // Should not throw, just do nothing
    BOOST_CHECK(vec.size() == 0);

    vec.reserve(1000000000000); // Should not throw, just do nothing
    BOOST_CHECK(vec.size() == 0);
    
    vec.resize(1000000000000); // Should not throw, just do nothing
    BOOST_CHECK(vec.size() == 0);
}