/**
 * @file  <class>.cpp
 * @brief <class>
 *
 * Class implementation for <class>
 *
 * @author     Clay Freeman
 * @date       March 14, 2015
 */

#include <memory>
#include <string>
#include "../include/DIE.hpp"
#include "../include/RawEvent.hpp"
#include "../../include/ConnectionManagement.hpp"
#include "../../include/EventHandling.hpp"
#include "../../include/Logger.hpp"
#include "../../include/Module.hpp"
#include "../../include/Runtime.hpp"
#include "../../include/SocketManagement.hpp"

/**
 * @brief Is Instantiated
 *
 * The method called directly after instantiation of this Module. This method is
 * used by the Module to prepare for loading
 *
 * @return true if loadable, false otherwise
 */
bool DIE::isInstantiated() {
  Logger::stack(__PRETTY_FUNCTION__);

  EventHandling::registerForEvent("rawEvent", this->getName(),
    &DIE::receiveRaw);

  Logger::stack(__PRETTY_FUNCTION__, true);
  return true;
}

/**
 * @brief Receive Raw
 *
 * Event callback for the RawEvent (provides a RawEventData struct)
 *
 * @remarks
 * Incoming data is a RawEventData struct (see modules/include/RawEvent.h)
 *
 * @param      name The name of the received event
 * @param[out] data A pointer to a RawEventData struct
 */
void DIE::receiveRaw(const std::string& name, void* data) {
  Logger::stack(__PRETTY_FUNCTION__);

  RawEventData* rawEventData = (RawEventData*)data;
  if (rawEventData->d == "DIE") {
    Logger::info(name + ": Shutting down...");
    for (auto i : ConnectionManagement::getConnections())
      i->send(name + ": Shutting down...\n");
    Runtime::add("__DIE__", "1");
  }

  Logger::stack(__PRETTY_FUNCTION__, true);
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
extern "C" Module* _load() { return new DIE; }
