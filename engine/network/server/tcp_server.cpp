#include "tcp_server.hpp"

#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

namespace Engine::Network {

TcpServer::TcpServer(){
  
  int sockfd, connfd, len; 
  struct sockaddr_in servaddr, cli; 

  // socket create and verification 
  sockfd = socket(AF_INET, SOCK_STREAM, 0); 
  if (sockfd == -1) { 
    std::cerr << "socket creation failed..." << '\n'; 
    return; 
  } 
  else
      std::cerr << "Socket successfully created.." << '\n'; 
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
  if ((::listen(sockfd, 5)) != 0) { 
    std::cerr << "Listen failed...\n"; 
    return; 
  } 
  else
    std::cerr << "Server listening..\n"; 
  len = sizeof(cli); 
   
}

TcpServer::~TcpServer() {}


void TcpServer::listenForConnections(){




}






} // namespace Engine::Network

