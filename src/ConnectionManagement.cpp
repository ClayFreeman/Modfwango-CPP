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
#include "../include/ConnectionManagement.hpp"

std::vector<std::shared_ptr<Connection>> ConnectionManagement::connections{};

/**
 * @brief Close All
 *
 * Closes all Connections held by ConnectionManagement
 */
void ConnectionManagement::closeAll() {
  ConnectionManagement::connections.clear();
}

/**
 * @brief Count
 *
 * Returns the number of Connections held by ConnectionManagement
 *
 * @return # of Connections
 */
int ConnectionManagement::count() {
  return ConnectionManagement::connections.size();
}

/**
 * @brief Get Connections
 *
 * Returns a reference to the std::vector holding all Connections
 *
 * @return A std::vector of Connections
 */
const std::vector<std::shared_ptr<Connection>>&
    ConnectionManagement::getConnections() {
  return ConnectionManagement::connections;
}

/**
 * @brief New Connection
 *
 * Adds the provided Connection to the internal std::vector of Connections
 *
 * @param c An std::shared_ptr<Connection>
 */
void ConnectionManagement::newConnection(const std::shared_ptr<Connection>& c) {
  ConnectionManagement::connections.push_back(c);
}

/**
 * @brief Prune Connections
 *
 * Destroys all invalid Connections
 */
void ConnectionManagement::pruneConnections() {
  for (auto it = ConnectionManagement::connections.begin();
      it != ConnectionManagement::connections.end(); ++it)
    if (!(*it)->isValid())
      ConnectionManagement::connections.erase(it--);
}
