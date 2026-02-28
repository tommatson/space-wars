#pragma once

#include "socket.hpp"

namespace Engine::Network {

class TcpSocket : public Socket {
public:

  
  bool openSocket(uint16_t port) override;
  void closeSocket() override;
  bool isOpen() const override;

  bool send(const char* destinationAddress, uint16_t destinationPort, const void* data, int size) override;

  int receive(char* senderAddressBuffer, int addressBufferSize, uint16_t& senderPort, void* data, int maxSize) override;



private:



};

} // namespace Engine::Network
