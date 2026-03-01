#include "tcp_server.hpp"


namespace Engine::Network {

TcpServer::TcpServer(){
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    std::cerr << "Socket creation failed" << std::endl;
    return;
  }

  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces
  address.sin_port = htons(8080);       // Port 8080

  if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
      std::cerr << "Bind failed" << std::endl;
      return;
  }




}


} // namespace Engine::Network

