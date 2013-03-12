#include "unitest.h"

#include <utils/logging.h>
using namespace Utils;

BOOST_AUTO_TEST_SUITE(LOGGING)

BOOST_AUTO_TEST_CASE(LOGGING)
{
    BOOST_CHECKPOINT("LOGGING");

    LOG::Level("TRACE");
    LOG::File("test.log");

    LOG(ERROR) << "This is a ERROR log.";
    LOG(WARN) << "This is a WARN log.";
    LOG(INFO) << "This is a info log.";
    LOG(DEBUG) << "This is a DEBUG log.";
    LOG(TRACE) << "This is a TRACE log.";
}

BOOST_AUTO_TEST_SUITE_END() /* LOGGING Test */