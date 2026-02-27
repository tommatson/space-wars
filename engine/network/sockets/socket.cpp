#include "socket.hpp"
#include <iostream>
#include <cstring>
#include <string>

#if defined(_WIN32)
  #pragma comment(lib, "ws2_32.lib")
  #include <winsock2.h>
  #include <ws2tcpip.h>
  typedef int socklen_t;
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <fcntl.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <unistd.h>
#endif

namespace Engine {
namespace Network {

bool initializeSockets() {
#if defined(_WIN32)
    WSADATA WsaData;
    return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
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
    // Create a dual-stack IPv6 socket (also accepts IPv4 via mapped addresses)
    m_socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (m_socket <= 0) {
        return false;
    }

    // Disable IPV6_V6ONLY so the socket accepts both IPv4 and IPv6 connections
    int no = 0;
    if (setsockopt(m_socket, IPPROTO_IPV6, IPV6_V6ONLY,
                   (const char*)&no, sizeof(no)) < 0) {
        closeSocket();
        return false;
    }

    sockaddr_in6 address{};
    address.sin6_family = AF_INET6;
    address.sin6_addr   = in6addr_any; // Binds to all interfaces (v4 + v6)
    address.sin6_port   = htons(port);

    if (bind(m_socket, (const sockaddr*)&address, sizeof(address)) < 0) {
        closeSocket();
        return false;
    }

    // Set non-blocking
#if defined(_WIN32)
    DWORD nonBlocking = 1;
    if (ioctlsocket(m_socket, FIONBIO, &nonBlocking) != 0) {
        closeSocket(); // BUG FIX: was calling close() instead of closeSocket()
        return false;
    }
#else
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (flags == -1) {
        closeSocket();
        return false;
    }
    if (fcntl(m_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        closeSocket();
        return false;
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

bool Socket::send(const char* destinationAddress, uint16_t destinationPort,
                  const void* data, int size) {
    if (!isOpen()) return false;

    addrinfo hints{}, *res = nullptr;
    hints.ai_family   = AF_UNSPEC;    // Resolve to whichever family the address is
    hints.ai_socktype = SOCK_DGRAM;

    std::string portStr = std::to_string(destinationPort);
    if (getaddrinfo(destinationAddress, portStr.c_str(), &hints, &res) != 0 || !res) {
        return false;
    }

    // If the destination resolved to an IPv4 address but our socket is AF_INET6,
    // we need to send as an IPv4-mapped IPv6 address so the dual-stack socket
    // can route it correctly on platforms that require this (e.g. Windows).
    sockaddr_storage dest{};
    socklen_t destLen = 0;

    if (res->ai_family == AF_INET) {
        // Wrap the IPv4 address into an IPv4-mapped IPv6 address
        auto* dest6 = reinterpret_cast<sockaddr_in6*>(&dest);
        auto* src4  = reinterpret_cast<sockaddr_in*>(res->ai_addr);
        dest6->sin6_family = AF_INET6;
        dest6->sin6_port   = src4->sin_port;
        // ::ffff:a.b.c.d  — the IPv4-mapped prefix
        dest6->sin6_addr.s6_addr[10] = 0xFF;
        dest6->sin6_addr.s6_addr[11] = 0xFF;
        std::memcpy(&dest6->sin6_addr.s6_addr[12],
                    &src4->sin_addr.s_addr, 4);
        destLen = sizeof(sockaddr_in6);
    } else {
        // Already IPv6, copy as-is
        std::memcpy(&dest, res->ai_addr, res->ai_addrlen);
        destLen = static_cast<socklen_t>(res->ai_addrlen);
    }

    freeaddrinfo(res);

    int sentBytes = sendto(m_socket, (const char*)data, size, 0,
                           (const sockaddr*)&dest, destLen);
    return sentBytes == size;
}

int Socket::receive(char* senderAddressBuffer, int addressBufferSize,
                    uint16_t& senderPort, void* data, int maxSize) {
    if (!isOpen()) return 0;

    sockaddr_storage from{};
    socklen_t fromLength = sizeof(from);

    int receivedBytes = recvfrom(m_socket, (char*)data, maxSize, 0,
                                 (sockaddr*)&from, &fromLength);

    if (receivedBytes > 0) {
        if (from.ss_family == AF_INET6) {
            auto* addr6 = reinterpret_cast<sockaddr_in6*>(&from);
            // Check if this is actually an IPv4-mapped address (::ffff:a.b.c.d)
            // and if so, unwrap it so callers always see a plain IPv4 string.
            const uint8_t* b = addr6->sin6_addr.s6_addr;
            bool isMapped = (b[0]==0 && b[1]==0 && b[2]==0  && b[3]==0  &&
                             b[4]==0 && b[5]==0 && b[6]==0  && b[7]==0  &&
                             b[8]==0 && b[9]==0 && b[10]==0xFF && b[11]==0xFF);
            if (isMapped) {
                // Re-cast as sockaddr_in over the IPv4 bytes for getnameinfo
                sockaddr_in mapped{};
                mapped.sin_family = AF_INET;
                mapped.sin_port   = addr6->sin6_port;
                std::memcpy(&mapped.sin_addr.s_addr, &b[12], 4);
                getnameinfo((sockaddr*)&mapped, sizeof(mapped),
                            senderAddressBuffer, addressBufferSize,
                            nullptr, 0, NI_NUMERICHOST);
                senderPort = ntohs(mapped.sin_port);
                return receivedBytes;
            }
        }

        // Native IPv4 or native IPv6 — let getnameinfo format it
        getnameinfo((sockaddr*)&from, fromLength,
                    senderAddressBuffer, addressBufferSize,
                    nullptr, 0, NI_NUMERICHOST);

        if (from.ss_family == AF_INET) {
            senderPort = ntohs(reinterpret_cast<sockaddr_in*>(&from)->sin_port);
        } else {
            senderPort = ntohs(reinterpret_cast<sockaddr_in6*>(&from)->sin6_port);
        }
    }

    return receivedBytes;
}

std::optional<Endpoint> Socket::getSocketAddress() {
    if (!isOpen()) return std::nullopt;

    sockaddr_in6 address{};
    socklen_t length = sizeof(address);

    if (getsockname(m_socket, (sockaddr*)&address, &length) < 0) {
        return std::nullopt;
    }

    Endpoint endpoint;
    endpoint.port = ntohs(address.sin6_port);


    std::memcpy(endpoint.ip.data(), &address.sin6_addr, 16);

    return endpoint;
}

} // namespace Network
} // namespace Engine
