#define BOOST_TEST_MODULE SAFE_NUMERIC_TEST
#include <boost/test/included/unit_test.hpp>
#include "numeric.hpp"

BOOST_AUTO_TEST_CASE(safe_numeric_basic) {
    safe::numeric<int> num1(42);
    BOOST_CHECK(num1 == 42);

    safe::numeric<int> num2;
    num2 = 100;
    BOOST_CHECK(num2 == 100);

    safe::numeric<int> num3(num1);
    BOOST_CHECK(num3 == 42);

    safe::numeric<int> num4(std::move(num2));
    BOOST_CHECK(num4 == 100);

    // BOOST_CHECK(!num2.has_value()); // num2 should be empty after move

    num4 = num3;
    BOOST_CHECK(num4 == 42);

    num4 = std::move(num3);
    BOOST_CHECK(num4 == 42);

    BOOST_CHECK(num1 < 43);    // And this

    num4 = std::numeric_limits<int>::max();
    
    BOOST_CHECK(num4 + num3 == std::nullopt); // Overflow
    BOOST_CHECK(num4 - 1);
    BOOST_CHECK(num3 - num4 == std::nullopt); // Underflow
    
    using safe::operator"" _su;
    using safe::operator"" _si;
    using safe::operator"" _sc;
    using safe::operator"" _suc;
    
    BOOST_CHECK(1_si/0 == std::nullopt); // Division by zero
    BOOST_CHECK(1_su - 10_su == std::nullopt);
    BOOST_CHECK(1_si - 10_si == -9_si);
    BOOST_CHECK(-1_su == std::nullopt);
    BOOST_CHECK(-1_su == -10_su);
    BOOST_CHECK(-1_si != -10_si);
    BOOST_CHECK(-1_si == -1);
    //BOOST_CHECK(1_si == 1_su); // Compile error: different types

    auto sum = 0_si;
    for(auto i = 0_si; i < 10_si; ++i) {
        BOOST_CHECK(i < 10_si);
        sum += i;
    }
    BOOST_CHECK(sum == 45); 

    BOOST_CHECK(255_suc == 255);
    BOOST_CHECK(256_suc == std::nullopt); // Overflow for unsigned char
}