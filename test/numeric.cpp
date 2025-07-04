#define BOOST_TEST_MODULE SAFE_NUMERIC_TEST
#include <boost/test/included/unit_test.hpp>
#include "numeric.hpp"

BOOST_AUTO_TEST_CASE(safe_numeric_basic) {
    safe::numeric<int> num1(42);
    BOOST_CHECK(num1 == 42);

    safe::numeric<int> num2;
    BOOST_CHECK(!num2.has_value());
    num2 = 100;
    BOOST_CHECK(num2 == 100);

    safe::numeric<int> num3(num1);
    BOOST_CHECK(num3 == 42);

    safe::numeric<int> num4(std::move(num2));
    BOOST_CHECK(num4 == 100);

    BOOST_CHECK(!num2.has_value()); // num2 should be empty after move

    num4 = num3;
    BOOST_CHECK(num4 == 42);

    num4 = std::move(num3);
    BOOST_CHECK(num4 == 42);
    BOOST_CHECK(num1 < 43);
    BOOST_CHECK(num3 == std::nullopt); // num3 should be empty after move

    num4 = safe::numeric<int>::max();
    
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

    BOOST_CHECK(255 == 255_suc);
    BOOST_CHECK(255_suc == 255);
    BOOST_CHECK(256_suc == std::nullopt); // Overflow for unsigned char

    BOOST_CHECK(safe::numeric<float>(NAN) == std::nullopt);
    // BOOST_CHECK(safe::numeric<int>(NAN) == std::nullopt);

    BOOST_CHECK(safe::numeric<int>("12") == 12);
    BOOST_CHECK(safe::numeric<int>("-12") == -12);
    BOOST_CHECK(safe::numeric<unsigned int>("-12") == std::nullopt); // Invalid conversion
    BOOST_CHECK(safe::numeric<float>("1.2") == 1.2);
    BOOST_CHECK(safe::numeric<double>("-1.22") == -1.22);
    BOOST_CHECK(safe::numeric<float>("bla") == std::nullopt); // Invalid conversion
    BOOST_CHECK(safe::numeric<int>("") == std::nullopt); // Empty string
    BOOST_CHECK(safe::numeric<unsigned char>("257") == std::nullopt); // Out of range for char
    BOOST_CHECK(safe::numeric<char>("128") == std::nullopt); // Out of range for char
    BOOST_CHECK(safe::numeric<char>("127") == 127);
    BOOST_CHECK(safe::numeric<int>("12345678901234567890") == std::nullopt); // Out of range for int

    bool called = false;
    safe::numeric<int>().for_each(
        [](int v) { BOOST_FAIL("Should not be called, no value present"); },
        [&called] { called = true; });
    BOOST_CHECK(called); 

    safe::numeric<int>(42).for_each(
        [](int v) { BOOST_CHECK(v == 42); },
        []{ BOOST_FAIL("Should not be called, value present"); }
    );
}