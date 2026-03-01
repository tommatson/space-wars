#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>


namespace Engine::Network {

class TcpServer {
public:

  TcpServer();
  ~TcpServer();

  TcpServer(const TcpServer&) = delete;
  TcpServer &operator=(const TcpServer&) = delete;



private:

};

} // namespace Engine::Network
