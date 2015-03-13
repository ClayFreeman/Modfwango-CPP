/**
 * @file  Socket.cpp
 * @brief Socket
 *
 * Class implementation for Socket
 *
 * @author     Clay Freeman
 * @date       March 11, 2015
 */

#include <arpa/inet.h>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "../include/Connection.h"
#include "../include/Logger.h"
#include "../include/Socket.h"

Socket::Socket(const std::string& addr, int portno): host{addr}, port{portno} {
  // Prepare the listen address
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family      = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(addr.c_str());
  serv_addr.sin_port        = htons(portno);

  // Setup the socket
  this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  // Set nonblocking mode (to be safe, not needed)
  fcntl(this->sockfd, F_SETFL, O_NONBLOCK);
  // Allow reusing the socket
  int yes = 1;
  setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  // Attempt to bind
  if (bind(this->sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    close(this->sockfd);
    throw std::runtime_error{"Couldn't bind to " + addr + ":" +
      std::to_string(portno)};
  }
  else {
    // Listen with a backlog of 1
    listen(this->sockfd, 1);
    Logger::debug("Listening on " + addr + ":" + std::to_string(portno));
  }
}

Socket::~Socket() {
  if (this->isValid()) {
    Logger::debug("Socket " + this->host + ":" + std::to_string(this->port) +
      " closed");
    close(this->sockfd);
  }
}

std::shared_ptr<Connection> Socket::acceptConnection() const {
  struct sockaddr_in cli_addr;
  socklen_t cli_addr_len = sizeof(cli_addr);
  memset(&cli_addr, 0, cli_addr_len);
  int cli_fd = -1;

  if (this->isValid()) {
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(this->sockfd, &rfds);
    struct timeval timeout{0, 0};
    select(this->sockfd + 1, &rfds, nullptr, nullptr, &timeout);

    if (FD_ISSET(this->sockfd, &rfds)) {
      cli_fd = accept(this->sockfd, (struct sockaddr*)&cli_addr, &cli_addr_len);
      if (cli_fd < 0) {
        throw std::runtime_error{"Couldn't accept client on " + this->host +
          ":" + std::to_string(this->port) +
          " - Invalid client file descriptor"};
      }
    }
    else {
      throw std::exception{};
    }
  }
  else {
    throw std::runtime_error{"Couldn't accept client on " + this->host +
      ":" + std::to_string(this->port) + " - Invalid socket"};
  }

  // Set nonblocking mode (to be safe, not needed)
  fcntl(this->sockfd, F_SETFL, O_NONBLOCK);

  Logger::debug("Accepted client " + std::string{inet_ntoa(cli_addr.sin_addr)} +
    " on " + this->host + ":" + std::to_string(this->port));
  return std::shared_ptr<Connection>{
    new Connection{inet_ntoa(cli_addr.sin_addr), this->port, cli_fd}
  };
}
