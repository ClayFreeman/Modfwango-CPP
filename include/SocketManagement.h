/**
 * @file  SocketManagement.h
 * @brief SocketManagement
 *
 * Class definition for SocketManagement
 *
 * @author     Clay Freeman
 * @date       March 13, 2015
 */

#ifndef _SOCKETMANAGEMENT_H
#define _SOCKETMANAGEMENT_H

#include <map>
#include <memory>
#include <string>
#include "../include/Socket.h"

class SocketManagement {
  private:
    static std::map<std::string, std::shared_ptr<Socket>> sockets;
    // Prevent this class from being instantiated
    SocketManagement() {}
  public:
    static void        acceptConnections();
    static bool        destroySocket(const std::string& addr, int port);
    static const std::map<std::string, std::shared_ptr<Socket>>& getSockets()
      { return SocketManagement::sockets; }
    static std::string getValidIP(const std::string& addr);
    static bool        isValidIP(const std::string& addr);
    static bool        newSocket(const std::string& addr, int port);
    static void        stall();
};

#endif
