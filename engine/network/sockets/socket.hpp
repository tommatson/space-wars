#pragma once
#include <cstdint>

namespace Engine {
namespace Network {

    bool initializeSockets();
    void shutdownSockets();

    class Socket {
    public:
        Socket();
        ~Socket();

        // Pass 0 for random port (for clients) 
        // or a specific port (for server)
        bool openSocket(uint16_t port);
        void closeSocket();
        bool isOpen() const;

        // Send raw bytes to an IP
        bool send(const char* destinationAddress, uint16_t destinationPort, const void* data, int size);
        
        // Receives raw bytes. Returns the number of bytes read, or <= 0 if nothing is there.
        int receive(char* senderAddressBuffer, int addressBufferSize, uint16_t& senderPort, void* data, int maxSize);

    private:
        int m_socket; 
    };

} // namespace Network
} // namespace Engine
