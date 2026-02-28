#pragma once
#include <cstdint>
#include <optional>
#include <string>

#include "../endpoint.hpp"

namespace Engine::Network{


class Socket {
public:
  
  Socket() : m_socket(0) {};
  virtual ~Socket() = default;


  virtual bool openSocket(uint16_t port) = 0;
  virtual void closeSocket() = 0;
  virtual bool isOpen() const = 0;

  virtual bool send(const char* destinationAddress, uint16_t destinationPort, const void* data, int size) = 0;
  
  virtual int receive(char* senderAddressBuffer, int addressBufferSize, uint16_t& senderPort, void* data, int maxSize) = 0;


protected:
  int m_socket;



};

} // namespace Engine::Network

