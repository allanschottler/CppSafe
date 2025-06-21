#define BOOST_TEST_MODULE SAFE_OPTIONAL_TEST
#include <boost/test/included/unit_test.hpp>
#include "optional.hpp"

BOOST_AUTO_TEST_CASE(safe_optional_basic) {
    safe::optional<int> opt1(42);
    BOOST_CHECK(opt1.has_value());
    opt1.and_then([](const int& val) { BOOST_CHECK(val == 42); });

    safe::optional<int> opt2;
    BOOST_CHECK(!opt2.has_value());
    BOOST_CHECK(opt2.and_then([](const int& val) { return val; }) == std::nullopt);

    opt2 = 100;
    BOOST_CHECK(opt2.has_value());
    BOOST_CHECK(opt2.and_then([](const int& val) { return val; }) == 100);
    opt2.and_then([](const int& val) { return val+1; })
        .and_then([](const int& val) { return val+10; })
        .and_then([](const int& val) { BOOST_CHECK(val == 111); });
    // BOOST_CHECK(*opt2.and_then([](const int& val) { return std::nullopt; })
    //                 .and_then([](const int& val) { return val+10; }) == std::nullopt);
    bool ok = false;
    opt2.and_then([&](const int& val) { ok = true; });
    BOOST_CHECK(ok);
}