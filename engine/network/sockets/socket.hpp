#pragma once
#include <cstdint>

class Socket {
public:
  Socket();

  ~Socket();

  void open(uint16_t port);


  

private:
  
  int m_socket;


};
