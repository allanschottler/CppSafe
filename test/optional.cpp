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

    // Move safe
    safe::optional<int> opt3;
    opt2 = std::move(opt1);
    BOOST_CHECK(opt2.has_value());
    BOOST_CHECK(!opt1.has_value()); // opt1 should be empty after move
    opt2.and_then([](const int& val) { BOOST_CHECK(val == 42); });

    opt2 = 100;
    opt2.and_then([](const int& val) { return val+1; })
        .and_then([](const int& val) { return val+10; })
        .and_then([](const int& val) { BOOST_CHECK(val == 111); });
    opt2.and_then([](const int& val) { BOOST_CHECK(val == 100); }); // Doesnt alter original value, that would be stateful

    // Unless...
    opt2 = 100;
    opt2.and_then([](const int& val) { return val+1; })
        .and_then([](const int& val) { return val+10; })
        .and_then([&opt2](const int& val) { opt2 = val; });
    opt2.and_then([](const int& val) { BOOST_CHECK(val == 111); }); // Now with new value
    
    // But...
    opt2 = 100;
    opt2.and_then([](const int& val) { return val+1; })
        .and_then([](const int& val) { return val+10; })
        .and_then([](const int& val) { BOOST_CHECK(val == 111); return val; })
        .and_then([&opt2](const int& val) { opt2 = 2; return val; }) // Propagates 111, but opt2 is now 2
        .and_then([](const int& val) { BOOST_CHECK(val == 111); }); // Receives 111
    opt2.and_then([](const int& val) { BOOST_CHECK(val == 2); }); // But now with new value


    // BOOST_CHECK(*opt2.and_then([](const int& val) -> safe::optional<int> { return std::nullopt; })
    //                 .and_then([](const int& val) { return val+10; }) == std::nullopt);
    bool ok = false;
    opt2.and_then([&](const int& val) { ok = true; });
    BOOST_CHECK(ok);
}

BOOST_AUTO_TEST_CASE(safe_optional_for_each) {
    safe::optional<int> opt1(42);
    bool called = false;
    opt1.for_each([&called](const int& val) { BOOST_CHECK(val == 42); called = true; },
                  []() { BOOST_FAIL("Should not call else branch"); });
    BOOST_CHECK(called);
    called = false;
    safe::optional<int> opt2;
    opt2.for_each([](const int& val) { BOOST_FAIL("Should not call then branch"); },
                  [&called]() { called = true; });
    BOOST_CHECK(called);
}