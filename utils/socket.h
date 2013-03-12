#ifndef _utils_SOCKET_H_
#define _utils_SOCKET_H_

namespace Utils {

class Socket 
{
public:
    static bool Init();
    static bool Close(size_t socket);
    static int GetFreePort(size_t expectedPort = 0);
};

} /* namespace Utils */

#endif /* _utils_SOCKET_H_ */
