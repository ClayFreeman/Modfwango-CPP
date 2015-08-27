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

#include <memory>
#include <string>
#include "FileDescriptor.hpp"

class Connection {
  private:
    std::string                     host   = "0.0.0.0";
    int                             port   = 0;
    std::shared_ptr<FileDescriptor> sockfd = nullptr;
    // Make sure copying is disallowed
    Connection(const Connection&);
    Connection& operator= (const Connection&);
    std::string& ltrim(std::string& s) const;
    void         reset();
    std::string& rtrim(std::string& s) const;
    std::string& trim(std::string& s) const;
  public:
    Connection(const std::string& addr, int portno,
        std::shared_ptr<FileDescriptor> sock):
      host{addr}, port{portno}, sockfd{sock} {}
    ~Connection();
    std::string                     getData();
    const std::string&              getHost() const;
    int                             getPort() const;
    std::shared_ptr<FileDescriptor> getSock() const;
    bool                            isValid() const;
    void                            send(const std::string& data) const;
};

#endif
