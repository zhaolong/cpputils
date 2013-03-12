#ifndef _utils_CONFIG_H_
#define _utils_CONFIG_H_

#if defined(_WIN32)
#   define OS_WIN32
#elif defined(__linux__)
#   define OS_LINUX
#else 
#   error unsupported platform. supported platform: win32, linux.
#endif

#endif /* _utils_CONFIG_H_ */