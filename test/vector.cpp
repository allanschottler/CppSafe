#define BOOST_TEST_MODULE SAFE_VECTOR_TEST
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <optional>
#include "vector.hpp"

BOOST_AUTO_TEST_CASE(safe_vector_basic) {
    safe::vector<int> vec{1, 2, 3};   

    auto opt_val = vec.at(1);
    BOOST_CHECK(opt_val.has_value());
    BOOST_CHECK(opt_val.value() == 2);

    opt_val = vec.at(5);
    BOOST_CHECK(!opt_val.has_value());
}

BOOST_AUTO_TEST_CASE(safe_vector_loop) {
    safe::vector<int> vec{1, 2, 3};  
    int sum = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if (auto val = *it) {
            sum += val.value();
        }
    }
    BOOST_CHECK(sum == 6);
    sum = 0;
    for(const auto& val : vec) {
        sum += val.value();
    }
    BOOST_CHECK(sum == 6);
}