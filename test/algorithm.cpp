#define BOOST_TEST_MODULE SAFE_ALGORITHM_TEST
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <optional>
#include "algorithm.hpp"
#include "vector.hpp"

BOOST_AUTO_TEST_CASE(safe_algorithm_find) {
    // std::vector<int> vec = {1, 2, 3, 4, 5};
    // auto it = safe::find(vec.begin(), vec.end(), 3);
    // BOOST_CHECK(it.has_value());
    // BOOST_CHECK(**it == 3);

    // it = safe::find(vec.begin(), vec.end(), 6);
    // BOOST_CHECK(!it.has_value());

    // safe::vector<int> svec = {1, 2, 3, 4, 5};
    // auto sit = safe::find(svec.begin(), svec.end(), 3);
    // BOOST_CHECK(it.has_value());
    // BOOST_CHECK(**it == 3);

    // sit = safe::find(svec.begin(), svec.end(), 6);
    // BOOST_CHECK(!sit.has_value());
}