#include "tcp_server.hpp"
#include <fcntl.h>
#include <cerrno>

#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

namespace Engine::Network {

TcpServer::TcpServer(){
  
  int connfd, len; 
  struct sockaddr_in servaddr, cli; 

  // socket create and verification 
  sockfd = socket(AF_INET, SOCK_STREAM, 0); 
  if (sockfd == -1) { 
    std::cerr << "socket creation failed..." << '\n'; 
    return; 
  } 
  else
      std::cerr << "Socket successfully created.." << '\n'; 

  int flags = fcntl(sockfd, F_GETFL, 0);
  if (flags == -1) {
    std::cerr << "fcntl F_GETFL failed...\n";
  } else {
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
      std::cerr << "fcntl F_SETFL failed...\n";
    } else {
      std::cerr << "Socket set to non-blocking...\n";
    }
  }

  memset(&servaddr, 0, sizeof(servaddr)); 

  // assign IP, PORT 
  servaddr.sin_family = AF_INET; 
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
  servaddr.sin_port = htons(PORT); 

  // Binding newly created socket to given IP and verification 
  if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
    std::cerr << "socket bind failed..." << '\n'; 
    return; 
  } 
  else
      std::cerr << "Socket successfully binded..." << '\n'; 

  // Now server is ready to listen and verification 
  if ((::listen(sockfd, 10)) != 0) { 
    std::cerr << "Listen failed...\n"; 
    return; 
  } 
  else
    std::cerr << "Server listening..\n"; 

   
}

TcpServer::~TcpServer() {}



void TcpServer::registerClientConnection(int connfd){

}


void TcpServer::listenForClients(){
  for (;;){
    struct sockaddr_in cli;
    socklen_t len = sizeof(cli);
    int connfd = accept(sockfd, (SA*)&cli, &len);

    if (connfd < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        break;   // No more pending connections
      else
        std::cerr << "Error in connection" << '\n';
    } else {

      // handle client



    }
  }
}







} // namespace Engine::Network

