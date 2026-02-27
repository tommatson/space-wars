#pragma once
#include <cstdint>

namespace Engine { namespace Network {

class Socket {
public:
  Socket();

  ~Socket();

  void open(uint16_t port);


  

private:
  
  int m_socket;


};

} } // namespace Engine::Network
