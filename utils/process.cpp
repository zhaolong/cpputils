#include "config.h"
#include "process.h"

#if        defined OS_WIN32
#include <Windows.h>
#elif    defined OS_LINUX
#endif

#include "string.h"

namespace Utils {

static void PosixPath(char* winpath) {String::Replace(winpath, "\\", "/");}
static void PosixPath(std::string& winpath) {String::Replace(winpath, "\\", "/");}

std::string CurrentProcess::Path()
{
    static const size_t size = 1024;
    char buf[size];
    int count = 0;

#if   defined(OS_WIN32)
    count = GetModuleFileName(NULL, buf, size);
    if(count == 0 || count == size)
        return "";
    PosixPath(buf);
#elif defined(OS_LINUX)
    count = readlink("/proc/self/exe", buf, size);
    if(count == -1 || count == size)
        return "";
#endif

    buf[count] = '\0';

    return buf;
}

std::string CurrentProcess::Path(const char* subpath) {
    std::string path = Directory();
    if(subpath != NULL)
        path.append(subpath);
#ifdef OS_WIN32
    PosixPath(path);
#endif
    return path;
}

bool Process::Create(Process::Info& pinfo, const char* path, const char* arguments, bool show)
{
    if(String::IsEmpty(path))
        return false;

    // splice path and arguments into cmdline.
    std::string cmdline = path;
    if(!String::IsEmpty(arguments))
    {
        cmdline += " ";
        cmdline += arguments;
    }

#if   defined(OS_WIN32)

    STARTUPINFO startupInfo;
    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);

    PROCESS_INFORMATION processInformation;
    memset(&processInformation, 0, sizeof(processInformation));

    BOOL result;
    result = ::CreateProcessA(path, const_cast<char*>(cmdline.c_str()), NULL, NULL, FALSE, 
        show ? NORMAL_PRIORITY_CLASS : CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInformation);
    if(result)
    {
        pinfo.handle = processInformation.hProcess;
        pinfo.pid    = processInformation.dwProcessId;

        CloseHandle(processInformation.hThread);
        return true;
    }
    else
        return false;

#elif defined(OS_LINUX)

    pid_t childpid = fork();
    if(childpid == 0) // child process
    {
        Utils::String::Vector args;
        Utils::String::Split(args, cmdline.c_str(), " ");

        const char* argv[args.size()];
        size_t i = 0;
        for (i = 0; i < args.size(); i++)
        {
            argv[i] = args[i].c_str();
        }
        argv[i] = NULL;
        int res = execv(path, const_cast<char**>(argv));
        if(res < 0)
        {
            printf("execv failed after fork.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if(childpid > 0)
    {
        pinfo.pid = childpid;
        sleep(1);
        return true;
    }
    else
        return false;
#endif

}

void Process::Close(const Process::Info& pinfo)
{
#if   defined(OS_WIN32)
    if(WaitForSingleObject(static_cast<HANDLE>(pinfo.handle), 1) == WAIT_TIMEOUT)
        TerminateProcess(static_cast<HANDLE>(pinfo.handle), 1);
    CloseHandle(static_cast<HANDLE>(pinfo.handle));
#elif defined(OS_LINUX)
    if (0 < pinfo.pid && 0 == waitpid((pid_t)pinfo.pid, NULL, WNOHANG))
    {
        kill(pinfo.pid, SIGKILL);
        waitpid((pid_t)pinfo.pid, NULL, WUNTRACED);
    }
#endif
}

bool Process::IsRunning(const Process::Info& pinfo)
{
#if   defined(OS_WIN32)
    if(WaitForSingleObject(static_cast<HANDLE>(pinfo.handle), 1) == WAIT_TIMEOUT)
#elif defined(OS_LINUX)
    if (0 < pinfo.pid && 0 == waitpid((pid_t)pinfo.pid, NULL, WNOHANG))
#endif
        return true;

    return false;
}

} /* namespace Utils */
