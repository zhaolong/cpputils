#define BOOST_AUTO_TEST_MAIN

#include "unitest.h"

#include <utils/socket.h>

BOOST_AUTO_TEST_SUITE(UtilsTest)

BOOST_AUTO_TEST_CASE(Socket)
{
    BOOST_CHECKPOINT("GetFreePort");

    BOOST_CHECK_EQUAL(Utils::Socket::GetFreePort(8080), 8080);
    BOOST_CHECK(Utils::Socket::GetFreePort(0) > 0);
    BOOST_CHECK(Utils::Socket::GetFreePort() > 0);
    BOOST_CHECK(Utils::Socket::GetFreePort(80) > 0);
    BOOST_CHECK_EQUAL(Utils::Socket::GetFreePort(80), 80);
}

BOOST_AUTO_TEST_SUITE_END() /* Utils Test */