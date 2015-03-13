/**
 * @file  Connection.h
 * @brief Connection
 *
 * Class definition for Connection
 *
 * @author     Clay Freeman
 * @date       March 11, 2015
 */

#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <errno.h>
#include <fcntl.h>
#include <string>

class Connection {
  private:
    std::string  host   = "0.0.0.0";
    int          port   = 0;
    int          sockfd = 0;
    // Make sure copying is disallowed
    Connection(const Connection&);
    Connection& operator= (const Connection&);
    std::string& ltrim(std::string& s) const;
    void         reset() const;
    std::string& rtrim(std::string& s) const;
    std::string& trim(std::string& s) const;
  public:
    Connection(const std::string& addr, int portno, int sock):
      host{addr}, port{portno}, sockfd{sock} {}
    ~Connection();
    std::string        getData() const;
    const std::string& getHost() const { return this->host; }
    int                getPort() const { return this->port; }
    int                getSock() const { return this->sockfd; }
    bool               isValid() const
      { return fcntl(this->sockfd, F_GETFD) != -1 || errno != EBADF; };
    void               send(const std::string& data) const;
};

#endif
