#include "unitest.h"

#include <utils/config.h>
#include <utils/process.h>

BOOST_AUTO_TEST_SUITE(Process)

BOOST_AUTO_TEST_CASE(CurrentProcess)
{
    BOOST_CHECKPOINT("CurrentProcess");

#if   defined(OS_WIN32)
    BOOST_CHECK_EQUAL(Utils::CurrentProcess::Name(), "unitest.exe");
#elif defined(OS_LINUX)
    BOOST_CHECK_EQUAL(Utils::CurrentProcess::Name(), "unitest");
#endif

    std::string path = Utils::CurrentProcess::Path("");
    BOOST_CHECK_EQUAL(path, Utils::CurrentProcess::Directory());

    path = Utils::CurrentProcess::Path("test/subpath/");
    BOOST_CHECK_EQUAL(path.find("//test"), std::string::npos);
    BOOST_CHECK_NE(path.find("/test/"), std::string::npos);
#ifdef OS_WIN32
    BOOST_CHECK_EQUAL(Utils::CurrentProcess::Path("test\\subpath\\"), path);
#endif

}

BOOST_AUTO_TEST_CASE(Process)
{
    BOOST_CHECKPOINT("Process");

    Utils::Process::Info pinfo;
#if     defined(OS_WIN32)
    BOOST_CHECK_EQUAL(Utils::Process::Create(pinfo, Utils::CurrentProcess::Path("Unitest.exe").c_str()), true);
#elif   defined(OS_LINUX)
    BOOST_CHECK_EQUAL(Utils::Process::Create(pinfo, Utils::CurrentProcess::Path("Unitest").c_str()), true);
#endif

    BOOST_CHECK_EQUAL(Utils::Process::IsRunning(pinfo), true);

#if     defined(OS_WIN32)
    Utils::Process::Close(pinfo);
#elif   defined(OS_LINUX)
    // unknown location(0): fatal error in "Process": child was killed; pid: 17521; uid: 0; exit value: 9
    // the error reason is boost test catched the SIGCHLD.
    // http://boost.2283326.n4.nabble.com/Testing-Help-with-process-child-td2600488.html
    // http://stackoverflow.com/questions/5325202/boostunit-test-case-fails-because-a-child-process-exits-with-nonzero
    Utils::Process::Close(pinfo);
#endif

    BOOST_CHECK_EQUAL(Utils::Process::IsRunning(pinfo), false);
}

BOOST_AUTO_TEST_SUITE_END() /* Process Test */