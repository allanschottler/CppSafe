#define BOOST_TEST_MODULE SAFE_MEMORY_TEST
#include <boost/test/included/unit_test.hpp>
#include "memory.hpp"

BOOST_AUTO_TEST_CASE(safe_pointer_basic) {
    safe::pointer<int> p1(new int(42));
    BOOST_CHECK(p1->has_value());
    p1->and_then([](const int& val) { BOOST_CHECK(val == 42); });
    p1.reset(); // Reset pointer, should delete the int
    BOOST_CHECK(!p1->has_value());

    safe::pointer<int> p2;
    BOOST_CHECK(!p2->has_value());
    p2->for_each([](const int& val) { BOOST_FAIL("Should not call then branch"); },
                 []{ BOOST_CHECK(true); }); // Should call else branch

    // Move safe
    p1 = safe::pointer<int>(new int(100));
    BOOST_CHECK(p1->has_value());
    safe::pointer<int> p3 = std::move(p1);
    BOOST_CHECK(!p1->has_value()); // p1 should be empty after move
    BOOST_CHECK(p3->has_value());
    p3->for_each([](const int& val) { BOOST_CHECK(val == 100); },
                 []{ BOOST_FAIL("Should not call else branch"); });
    p3.reset(); // Reset pointer, should delete the int
    BOOST_CHECK(!p3->has_value());

    auto p = new int(200);
    safe::pointer<int> p4(std::move(p));
    BOOST_CHECK(!p);
    BOOST_CHECK(p4->has_value());
    p4->and_then([](const int& val) { BOOST_CHECK(val == 200); });
}

BOOST_AUTO_TEST_CASE(safe_pointer_unique) {
    safe::unique_ptr<int> up1(new int(42));
    BOOST_CHECK(up1->has_value());
    up1->and_then([](const int& val) { BOOST_CHECK(val == 42); });
    up1.reset(); // Reset pointer, should delete the int
    BOOST_CHECK(!up1->has_value());

    safe::unique_ptr<int> up2;
    BOOST_CHECK(!up2->has_value());
    up2->for_each([](const int& val) { BOOST_FAIL("Should not call then branch"); },
                  []{ BOOST_CHECK(true); }); // Should call else branch

    // Move safe
    up1 = safe::unique_ptr<int>(new int(100));
    BOOST_CHECK(up1->has_value());
    safe::unique_ptr<int> up3 = std::move(up1);
    BOOST_CHECK(!up1->has_value()); // up1 should be empty after move
    BOOST_CHECK(up3->has_value());
    up3->for_each([](const int& val) { BOOST_CHECK(val == 100); },
                  []{ BOOST_FAIL("Should not call else branch"); });
    up3.reset(); // Reset pointer, should delete the int
    BOOST_CHECK(!up3->has_value());
}