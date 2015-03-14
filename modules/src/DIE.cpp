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
#include "../include/DIE.h"
#include "../include/RawEvent.h"
#include "../../include/ConnectionManagement.h"
#include "../../include/EventHandling.h"
#include "../../include/Logger.h"
#include "../../include/Module.h"
#include "../../include/SocketManagement.h"

/**
 * @brief Is Instantiated
 *
 * The method called directly after instantiation of this Module. This method is
 * used by the Module to prepare for loading
 *
 * @return true if loadable, false otherwise
 */
bool DIE::isInstantiated() {
  Logger::stack(__FUNCTION__);

  EventHandling::registerForEvent("rawEvent", this->getName(),
    &DIE::receiveRaw);

  Logger::stack(__FUNCTION__, true);
  return true;
}

void DIE::receiveRaw(const std::string& name, void* data) {
  Logger::stack(__FUNCTION__);

  RawEventData* rawEventData = (RawEventData*)data;
  if (rawEventData->d == "DIE") {
    rawEventData->c->send(name + "\n");
    ConnectionManagement::closeAll();
    SocketManagement::closeAll();
  }

  Logger::stack(__FUNCTION__, true);
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
