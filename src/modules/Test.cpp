/**
 * @file  Test.cpp
 * @brief Test
 *
 * A test Module for the Modfwango-C++ framework
 *
 * @author     Clay Freeman
 * @date       January 23, 2015
 */

#include <sstream>
#include <string>
#include <unistd.h>
#include "../../include/Connection.h"
#include "../../include/ConnectionManagement.h"
#include "../../include/EventHandling.h"
#include "../../include/Logger.h"
#include "../../include/Module.h"
#include "../../include/Socket.h"
#include "../../include/SocketManagement.h"

class Test : public Module {
  public:
    // Initialize the name property
    Test() { this->setName("Test"); }
    // Demonstrate overriding the destructor
    ~Test() {}
    // Overload the isInstantiated() method
    bool isInstantiated();
};

/**
 * @brief Is Instantiated
 *
 * The method called directly after instantiation of this Module. This method is
 * used by the Module to prepare for loading
 *
 * @return true if loadable, false otherwise
 */
bool Test::isInstantiated() {
  Logger::stack(__FUNCTION__);

  Logger::debug("Begin Socket test");
  bool running = true;
  SocketManagement::newSocket("0.0.0.0", 1337);
  while (running == true) {
    SocketManagement::stall();
    SocketManagement::acceptConnections();
    ConnectionManagement::pruneConnections();
    for (auto i : ConnectionManagement::getConnections()) {
      std::string data;
      try {
        data = i->getData();
        if (data == "DIE")
          running = false;
      }
      catch (const std::runtime_error&) {}
    }
  }
  Logger::debug("End Socket test");

  Logger::stack(__FUNCTION__, true);
  return true;
}

/**
 * @brief Load
 *
 * Makes the Module available through dlsym()
 *
 * @remarks
 * The memory for this Module must be freed when unloaded
 *
 * @return A pointer to this Module
 */
extern "C" Module* _load() { return new Test; }
