/**
 * @file  RawEvent.cpp
 * @brief RawEvent
 *
 * A test Module for the Modfwango-C++ framework
 *
 * @author     Clay Freeman
 * @date       January 23, 2015
 */

#include <memory>
#include <string>
#include "../include/RawEvent.hpp"
#include "../../include/Connection.hpp"
#include "../../include/EventHandling.hpp"
#include "../../include/Module.hpp"
#include "../../include/Logger.hpp"

/**
 * @brief Is Instantiated
 *
 * The method called directly after instantiation of this Module. This method is
 * used by the Module to prepare for loading
 *
 * @return true if loadable, false otherwise
 */
bool RawEvent::isInstantiated() {
  Logger::stack(__PRETTY_FUNCTION__);

  EventHandling::createEvent("rawEvent", this->getName(),
    &RawEvent::receiveRaw);

  Logger::stack(__PRETTY_FUNCTION__, true);
  return true;
}

/**
 * @brief Receive Raw
 *
 * Event data callback to intercept all incoming data (data from Sockets held by
 * SocketManagement)
 *
 * @param      name       The name of the received event
 * @param[out] connection A pointer to the Connection from which the data was
 *                        received
 * @param      data       The data that was received
 */
void RawEvent::receiveRaw(const std::string& name,
    std::shared_ptr<Connection> connection, std::string data) {
  Logger::stack(__PRETTY_FUNCTION__);

  RawEventData rawEventData{connection, data};
  EventHandling::triggerEvent(name, (void*)&rawEventData);

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
extern "C" Module* _load() { return new RawEvent; }
