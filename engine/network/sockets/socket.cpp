#include "socket.hpp"
#include <iostream>
#include <cstring>

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

namespace Engine {
namespace Network {

bool initializeSockets() {
#if defined(_WIN32)
    WSADATA WsaData;
    return WSAStartup(MAKEWORD(2,2), &WsaData) == NO_ERROR;
#else
    return true; 
#endif
}

void shutdownSockets() {
#if defined(_WIN32)
    WSACleanup();
#endif
}

Socket::Socket() : m_socket(0) {}

Socket::~Socket() {
    closeSocket();
}

bool Socket::openSocket(uint16_t port) {
    // Create UDP socket
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_socket <= 0) {
        return false;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(m_socket, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0) {
        closeSocket();
        return false;
    }

    // Set Non-Blocking
#if defined(_WIN32)
    DWORD nonBlocking = 1;
    if (ioctlsocket(m_socket, FIONBIO, &nonBlocking) != 0) {
        close(); return false;
    }
#else
    int nonBlocking = 1;
    if (fcntl(m_socket, F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
        closeSocket(); return false;
    }
#endif

    return true;
}

void Socket::closeSocket() {
    if (m_socket != 0) {
#if defined(_WIN32)
        closesocket(m_socket);
#else
        close(m_socket);
#endif
        m_socket = 0;
    }
}

bool Socket::isOpen() const {
    return m_socket != 0;
}

bool Socket::send(const char* destinationAddress, uint16_t destinationPort, const void* data, int size) {
    if (!isOpen()) return false;

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(destinationAddress);
    address.sin_port = htons(destinationPort);

    int sent_bytes = sendto(m_socket, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));
    return sent_bytes == size;
}

int Socket::receive(char* senderAddressBuffer, int addressBufferSize, uint16_t& senderPort, void* data, int maxSize) {
    if (!isOpen()) return 0;

    sockaddr_in from;
    socklen_t fromLength = sizeof(from);

    int received_bytes = recvfrom(m_socket, (char*)data, maxSize, 0, (sockaddr*)&from, &fromLength);

    if (received_bytes > 0) {
        // Translate the sender's IP back into a readable string
        std::string ipStr = inet_ntoa(from.sin_addr);
        strncpy(senderAddressBuffer, ipStr.c_str(), addressBufferSize);
        senderPort = ntohs(from.sin_port);
    }

    return received_bytes;
}

} // namespace Network
} // namespace Engine
