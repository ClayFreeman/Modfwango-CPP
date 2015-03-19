/**
 * @file  ConnectionManagement.h
 * @brief ConnectionManagement
 *
 * Class definition for ConnectionManagement
 *
 * @author     Clay Freeman
 * @date       March 13, 2015
 */

#ifndef _CONNECTIONMANAGEMENT_H
#define _CONNECTIONMANAGEMENT_H

#include <memory>
#include <vector>
#include "Connection.hpp"

class ConnectionManagement {
  private:
    static std::vector<std::shared_ptr<Connection>> connections;
    // Prevent this class from being instantiated
    ConnectionManagement() {}
  public:
    static int  count();
    static void closeAll();
    static const std::vector<std::shared_ptr<Connection>>& getConnections();
    static void newConnection(const std::shared_ptr<Connection>& c);
    static void pruneConnections();
};

#endif
