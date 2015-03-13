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
#include "Connection.h"

class ConnectionManagement {
  private:
    static std::vector<std::shared_ptr<Connection>> connections;
    // Prevent this class from being instantiated
    ConnectionManagement() {}
  public:
    static const std::vector<std::shared_ptr<Connection>>& getConnections()
      { return ConnectionManagement::connections; }
    static void newConnection(const std::shared_ptr<Connection>& c)
      { ConnectionManagement::connections.push_back(c); }
    static void pruneConnections();
};

#endif
