#pragma once
#include <cstdint>
#include <optional>
#include <string>

#include "../endpoint.hpp"

namespace Engine::Network{


class Socket {
public:
  
  Socket() : m_socket(0) {};
  ~Socket() {closeSocket();};


  virtual bool openSocket(uint16_t port);
  virtual void closeSocket();
  virtual bool isOpen() const;

  virtual bool send(const char* destinationAddress, uint16_t destinationPort, const void* data, int size);
  
  virtual int receive(char* senderAddressBuffer, int addressBufferSize, uint16_t& senderPort, void* data, int maxSize);


protected:
  int m_socket;



};

} // namespace Engine::Network

