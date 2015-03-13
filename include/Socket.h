/**
 * @file  Socket.h
 * @brief Socket
 *
 * Class definition for Socket
 *
 * @author     Clay Freeman
 * @date       March 11, 2015
 */

#ifndef _SOCKET_H
#define _SOCKET_H

#include <errno.h>
#include <fcntl.h>
#include <memory>
#include <string>
#include "Connection.h"

class Socket {
  private:
    std::string host   = "0.0.0.0";
    int         port   = 0;
    int         sockfd = 0;
    // Make sure copying is disallowed
    Socket(const Socket&);
    Socket& operator= (const Socket&);
  public:
    Socket(const std::string& addr, int portno);
    ~Socket();
    std::shared_ptr<Connection> acceptConnection() const;
    std::string getHost() const { return this->host; }
    int         getPort() const { return this->port; }
    int         getSock() const { return this->sockfd; }
    bool        isValid() const { return fcntl(this->sockfd, F_GETFD) != -1 ||
                              errno != EBADF; };
};

#endif
