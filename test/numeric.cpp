#define BOOST_TEST_MODULE SAFE_NUMERIC_TEST
#include <boost/test/included/unit_test.hpp>
#include "numeric.hpp"

BOOST_AUTO_TEST_CASE(safe_numeric_basic) {
    safe::numeric<int> num1(42);
    BOOST_CHECK(num1.get() == 42);
    BOOST_CHECK(*num1 == 42);

    safe::numeric<int> num2;
    num2 = 100;
    BOOST_CHECK(num2.get() == 100);
    BOOST_CHECK(*num2 == 100);

    safe::numeric<int> num3(num1);
    BOOST_CHECK(num3.get() == 42);
    BOOST_CHECK(*num3 == 42);

    safe::numeric<int> num4(std::move(num2));
    BOOST_CHECK(num4.get() == 100);
    BOOST_CHECK(*num4 == 100);

    // BOOST_CHECK(!num2.has_value()); // num2 should be empty after move

    num4 = num3;
    BOOST_CHECK(num4.get() == 42);
    BOOST_CHECK(*num4 == 42);

    num4 = std::move(num3);
    BOOST_CHECK(num4.get() == 42);
    BOOST_CHECK(*num4 == 42);

    //BOOST_CHECK(43 > num1);  // Error
    BOOST_CHECK(43 > *num1);   // But this works
    BOOST_CHECK(num1 < 43);    // And this

    num4 = std::numeric_limits<int>::max();
    auto result = num4 + num3;
    BOOST_CHECK(!result.has_value());
    BOOST_CHECK((num4-1).has_value());
    // BOOST_CHECK(!(num3-num4).has_value()); // Underflow

    BOOST_CHECK(safe::numeric<int>(1)/0 == std::nullopt); // Division by zero
}