#include "socket.hpp"

#if defined(_WIN32)
    #pragma comment(lib, "wsock32.lib")
    #include <winsock2.h>
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <fcntl.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

void Socket::open(uint16_t port){

}

