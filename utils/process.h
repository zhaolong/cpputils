#ifndef _utils_PROCESS_H_
#define _utils_PROCESS_H_

#include <string>

namespace Utils {

class CurrentProcess 
{
public:
    static std::string Path();

    static inline std::string Directory() 
    {
        std::string path = Path();
        return path.substr(0, path.find_last_of('/') + 1);
    }
    static inline std::string Name() 
    {
        std::string path = Path();
        return path.substr(path.find_last_of('/') + 1);
    }

    static std::string Path(const char* subpath);
};

class Process 
{
public:
    struct Info 
    {
        size_t pid;
        void* handle; // WIN32 only.
    };

    static bool Create(Process::Info& pinfo, const char* path, const char* arguments = NULL, bool show = false);
    static void Close(const Process::Info& pinfo);
    static bool IsRunning(const Process::Info& pinfo);

};

} /* namespace Utils */

#endif /* _utils_PROCESS_H_ */
