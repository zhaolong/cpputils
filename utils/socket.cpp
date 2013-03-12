#include "config.h"
#include "socket.h"

#include <stdio.h>

#if        defined OS_WIN32
#include <Windows.h>
#elif    defined OS_LINUX
#endif

namespace Utils {

#ifdef OS_WIN32
class WinSocketIniter 
{
public:
    WinSocketIniter()
    {
        m_initFailed = !Init();
    }
    ~WinSocketIniter()
    {
        /* The Winsock DLL is acceptable. Proceed to use it. */
        /* Add network programming using Winsock here */
        /* then call WSACleanup when done using the Winsock dll */            
        WSACleanup();
    }

    inline bool Failed() { return m_initFailed; }
private:
    bool m_initFailed;

    bool Init()
    {
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;

        /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
        wVersionRequested = MAKEWORD(2, 2);

        err = WSAStartup(wVersionRequested, &wsaData);
        if (err != 0) {
            /* Tell the user that we could not find a usable */
            /* Winsock DLL.                                  */
            printf("WSAStartup failed with error: %d\n", err);
            return false;
        }

        /* Confirm that the WinSock DLL supports 2.2.*/
        /* Note that if the DLL supports versions greater    */
        /* than 2.2 in addition to 2.2, it will still return */
        /* 2.2 in wVersion since that is the version we      */
        /* requested.                                        */
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
            /* Tell the user that we could not find a usable */
            /* WinSock DLL.                                  */
            printf("Could not find a usable version of Winsock.dll\n");
            WSACleanup();
            return false;
        }
        else
            return true;

    }
}; /* WinSocketIniter */
#endif /* OS_WIN32 */

bool Socket::Init()
{
#if     defined(OS_WIN32)
    static WinSocketIniter wsIniter;
    return !wsIniter.Failed();
#elif   defined(OS_LINUX)
    return true;
#endif
}

inline bool Socket::Close(size_t socket)
{
    if(!Socket::Init())
        return false;
    int res;
#if     defined(OS_WIN32)
    res = ::closesocket(socket);
#elif   defined(OS_LINUX)
    res = ::close(socket);
#endif

    return (res == 0);
}

int Socket::GetFreePort(size_t expectedPort /* = 0 */)
{
    if(!Socket::Init())
        return -1;

    // Create a socket.
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        return -1;

    int port = static_cast<int>(expectedPort);

    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);

    // Check whether the expectedPort is valid.
    if(0 != port && 0 == ::bind(sock, (struct sockaddr *)&sin, sizeof(sin)))
    {
        // Close the socket.
        Close(sock);
        return port;
    }

    // For TCP/IP, if the port is specified as zero, 
    // the service provider assigns a unique port to the application from the dynamic client port range.
    sin.sin_port = htons(0);
    if (0 != ::bind(sock, (struct sockaddr *)&sin, sizeof(sin)))
    {
        // Close the socket.
        Close(sock);
        return -1;
    }

#if     defined(OS_WIN32)
    int len = sizeof(sin);
#elif   defined(OS_LINUX)
    size_t len = sizeof(sin);
#endif

    if(::getsockname(sock, (struct sockaddr *)&sin, &len) != 0)
    {
        // Close the socket.
        Close(sock);
        return -1;
    }

    port = ntohs(sin.sin_port);
    Close(sock);
    return port;

}

} /* namespace Utils */