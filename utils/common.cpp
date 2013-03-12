#include "config.h"
#include "common.h"

#if        defined(OS_WIN32)
#include <windows.h>
#elif    defined(OS_LINUX)
#include <unistd.h>
#endif

void Utils::Sleep(unsigned long miliseconds)
{
#if        defined(OS_WIN32)
    ::Sleep(miliseconds);
#elif    defined(OS_LINUX)
    ::Sleep(miliseconds);
#endif
}
