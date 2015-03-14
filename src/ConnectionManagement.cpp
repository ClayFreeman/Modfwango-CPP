/**
 * @file  ConnectionManagement.cpp
 * @brief ConnectionManagement
 *
 * Class implementation for ConnectionManagement
 *
 * @author     Clay Freeman
 * @date       March 13, 2015
 */

#include <memory>
#include <vector>
#include "../include/ConnectionManagement.h"

std::vector<std::shared_ptr<Connection>> ConnectionManagement::connections{};

void ConnectionManagement::closeAll() {
  ConnectionManagement::connections.clear();
}

int ConnectionManagement::count() {
  return ConnectionManagement::connections.size();
}

const std::vector<std::shared_ptr<Connection>>&
    ConnectionManagement::getConnections() {
  return ConnectionManagement::connections;
}

void ConnectionManagement::newConnection(const std::shared_ptr<Connection>& c) {
  ConnectionManagement::connections.push_back(c);
}

void ConnectionManagement::pruneConnections() {
  for (auto it = ConnectionManagement::connections.begin();
      it != ConnectionManagement::connections.end(); ++it)
    if (!(*it)->isValid())
      ConnectionManagement::connections.erase(it--);
}
