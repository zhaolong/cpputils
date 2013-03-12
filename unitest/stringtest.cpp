#include "unitest.h"

#include <utils/string.h>

BOOST_AUTO_TEST_SUITE(String)

BOOST_AUTO_TEST_CASE(LeftShift)
{
    BOOST_CHECKPOINT("LeftShift");

    char str[] = "LeftShift";

    Utils::String::LeftShift(str, 4);
    BOOST_CHECK_EQUAL(str, "Shift");

    Utils::String::LeftShift(str, 6);
    BOOST_CHECK_EQUAL(str, "");
}

BOOST_AUTO_TEST_CASE(Replace)
{
    BOOST_CHECKPOINT("Replace");

    std::string str = "C:\\Users\\Admin\\Desktop\\";
    Utils::String::Replace(str, "\\", "/");
    BOOST_CHECK_EQUAL(str, "C:/Users/Admin/Desktop/");

    Utils::String::Replace(str, "Admin", "Administrator");
    BOOST_CHECK_EQUAL(str, "C:/Users/Administrator/Desktop/");

    Utils::String::Replace(str, "Desktop/", "Desk");
    BOOST_CHECK_EQUAL(str, "C:/Users/Administrator/Desk");

    Utils::String::Replace(str, "Desk", "Desktop/");
    BOOST_CHECK_EQUAL(str, "C:/Users/Administrator/Desktop/");

    char cstr[] = "C:/Users/Administrator/Desktop/";
    Utils::String::Replace(cstr, "Administrator", "Admin");
    BOOST_CHECK_EQUAL(cstr, "C:/Users/Admin/Desktop/");

    Utils::String::Replace(cstr, "Admin", "Administrator");
    BOOST_CHECK_EQUAL(cstr, "C:/Users/Admin/Desktop/");

    Utils::String::Replace(cstr, "/", "\\");
    BOOST_CHECK_EQUAL(cstr, "C:\\Users\\Admin\\Desktop\\");
}

BOOST_AUTO_TEST_CASE(Split)
{
    BOOST_CHECKPOINT("Split");

    const char* str = "This is a String::Split() test.";
    Utils::String::Vector strvec;
    Utils::String::Split(strvec, str, " ");
    BOOST_CHECK_EQUAL(strvec[3], "String::Split()");
}

BOOST_AUTO_TEST_CASE(Number)
{
    BOOST_CHECKPOINT("Number");
    BOOST_CHECK_EQUAL(Utils::ToString(1024), "1024");
}

BOOST_AUTO_TEST_CASE(Equal)
{
    BOOST_CHECKPOINT("Equal");
    BOOST_CHECK_EQUAL(Utils::String::Equal("Equal", "Equal"), true);
    BOOST_CHECK_EQUAL(Utils::String::Equal("Equal", "Equas"), false);
    BOOST_CHECK_EQUAL(Utils::String::Equal("Equal", "equal"), true);
    BOOST_CHECK_EQUAL(Utils::String::Equal("Equal", "equal", false), false);
    BOOST_CHECK_EQUAL(Utils::String::Equal("", "Equal"), false);
    BOOST_CHECK_EQUAL(Utils::String::Equal("", ""), true);
    BOOST_CHECK_EQUAL(Utils::String::Equal("equal", ""), false);
    BOOST_CHECK_EQUAL(Utils::String::Equal("equall", "equal", true, strlen("equal")), true);
    BOOST_CHECK_EQUAL(Utils::String::Equal("Equall", "equal", true, strlen("equal")), true);
    BOOST_CHECK_EQUAL(Utils::String::Equal("equal", "Equall", true, strlen("equal")), true);
    BOOST_CHECK_EQUAL(Utils::String::Equal("equal", "Equall", true, strlen("Equall")), false);
}

BOOST_AUTO_TEST_CASE(StartsWith)
{
    BOOST_CHECKPOINT("StartsWith");
    BOOST_CHECK_EQUAL(Utils::String::StartsWith("StartsWith", "Start"), true);
    BOOST_CHECK_EQUAL(Utils::String::StartsWith("StartsWith", "StartsWith"), true);
    BOOST_CHECK_EQUAL(Utils::String::StartsWith("", ""), true);
    BOOST_CHECK_EQUAL(Utils::String::StartsWith("startsWith", "Start"), true);
    BOOST_CHECK_EQUAL(Utils::String::StartsWith("StartsWith", "start"), true);
    BOOST_CHECK_EQUAL(Utils::String::StartsWith("StartsWith", "start", false), false);
    BOOST_CHECK_EQUAL(Utils::String::StartsWith("Start", "StartsWith"), false);
}

BOOST_AUTO_TEST_CASE(EndsWith)
{
    BOOST_CHECKPOINT("EndsWith");
    BOOST_CHECK_EQUAL(Utils::String::EndsWith("EndsWith", "With"), true);
    BOOST_CHECK_EQUAL(Utils::String::EndsWith("EndsWitt", "With"), false);
    BOOST_CHECK_EQUAL(Utils::String::EndsWith("EndsWith", "Witt"), false);
    BOOST_CHECK_EQUAL(Utils::String::EndsWith("EndsWith", "Vitt"), false);
    BOOST_CHECK_EQUAL(Utils::String::EndsWith("EndsWith", "with"), true);
    BOOST_CHECK_EQUAL(Utils::String::EndsWith("EndsWith", "with", false), false);
    BOOST_CHECK_EQUAL(Utils::String::EndsWith("With", "EndsWith", false), false);
    BOOST_CHECK_EQUAL(Utils::String::EndsWith("", ""), true);
    BOOST_CHECK_EQUAL(Utils::String::EndsWith("EndsWith", "EndsWith"), true);
    BOOST_CHECK_EQUAL(Utils::String::EndsWith("EndsWith", ""), true);
    BOOST_CHECK_EQUAL(Utils::String::EndsWith("", "EndsWith"), false);
    BOOST_CHECK_EQUAL(Utils::String::EndsWith(NULL, NULL), false);
}

BOOST_AUTO_TEST_CASE(RemoveBlanks)
{
    BOOST_CHECKPOINT("RemoveBlanks");
    std::string testStr = " B L A N K S ";
    Utils::String::RemoveBlanks(testStr);
    BOOST_CHECK_EQUAL(testStr.c_str(), "BLANKS");

    testStr = "";
    Utils::String::RemoveBlanks(testStr);
    BOOST_CHECK_EQUAL(testStr.c_str(), "");

    testStr = "     ";
    Utils::String::RemoveBlanks(testStr);
    BOOST_CHECK_EQUAL(testStr.c_str(), "");

    testStr = "b\rl\na\tn k";
    Utils::String::RemoveBlanks(testStr);
    BOOST_CHECK_EQUAL(testStr.c_str(), "blank");
}

BOOST_AUTO_TEST_CASE(RemoveBlankSuffix)
{
    BOOST_CHECKPOINT("RemoveBlankSuffix");
    std::string testStr = " B L A N K S ";
    Utils::String::RemoveBlankSuffix(testStr);
    BOOST_CHECK_EQUAL(testStr.c_str(), " B L A N K S");
}

BOOST_AUTO_TEST_SUITE_END() /* StringTest */