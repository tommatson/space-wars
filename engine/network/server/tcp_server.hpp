#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>

#include <iostream>
#include <unordered_map>


namespace Engine::Network {

class TcpServer {
public:

  TcpServer();
  ~TcpServer();

  TcpServer(const TcpServer&) = delete;
  TcpServer &operator=(const TcpServer&) = delete;


  void listenForConnections();

  void listen();

  void sentToAllClients();

  void sendToClient(uint8_t playerId);


private:

  struct client {
    int connfd;
    uint8_t playerId;
  };

  std::unordered_map<uint8_t, int> clientMap;
};

} // namespace Engine::Network
