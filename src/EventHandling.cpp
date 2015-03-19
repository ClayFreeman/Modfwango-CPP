/**
 * @file  EventHandling.h
 * @brief EventHandling
 *
 * Class implementation for EventHandling
 *
 * @author     Clay Freeman
 * @date       February 19, 2015
 */

#include <map>
#include <memory>
#include <string>
#include "../include/Connection.hpp"
#include "../include/Event.hpp"
#include "../include/EventHandling.hpp"
#include "../include/EventPreprocessor.hpp"
#include "../include/EventRegistration.hpp"
#include "../include/Logger.hpp"
#include "../include/ModuleManagement.hpp"

// Initialize the events map
std::map<std::string, std::shared_ptr<Event>> EventHandling::events{};

/**
 * @brief Create Event
 *
 * Creates an Event with the provided optional data callback and owned by
 * the provided Module
 *
 * @param name         The name of the Event to be created
 * @param parentModule The name of the owning Module (can be empty if created
 *   by the framework, default = "")
 * @param callback     An optional function pointer to a function that accepts
 *   the Event name and data for processing (default = nullptr)
 *
 * @return true if the Event was created, false otherwise
 */
bool EventHandling::createEvent(const std::string& name,
    const std::string& parentModule, void (*callback)(const std::string&,
    std::shared_ptr<Connection>, std::string)) {
  bool status = false;
  // Prevent duplicate event names and make sure the parentModule exists (if
  // specified)
  if (name.length() > 0 && EventHandling::events.count(name) == 0 &&
      (parentModule.length() == 0 ||
        ModuleManagement::getModuleByName(parentModule).get() != nullptr)) {
    Logger::debug("Creating Event \"" + name + "\" ...");
    // Create and insert the Event into the events map
    EventHandling::events[name] = std::shared_ptr<Event>{
      new Event{name, parentModule, callback}
    };
    // Return a true status upon creation
    status = true;
  }
  else Logger::debug("Problem creating Event \"" + name
    + "\"");
  return status;
}

/**
 * @brief Destroy Event
 *
 * Destroys the Event with the provided name, along with all of its
 * associated internal data
 *
 * @param name The name of the Event to be destroyed
 *
 * @return true if the Event was found and destroyed, false otherwise
 */
bool EventHandling::destroyEvent(const std::string& name) {
  Logger::debug("Destroying Event \"" + name
    + "\" ...");
  return EventHandling::events.erase(name) > 0;
}

/**
 * @brief Receive Data
 *
 * Triggers each Event's data callback with the provided Connection and data
 *
 * @param c    The Connection in which the data was received
 * @param data The data received
 */
void EventHandling::receiveData(const std::shared_ptr<Connection>& c,
    const std::string& data) {
  for (auto i : EventHandling::events) {
    i.second->call(c, data);
  }
}

/**
 * @brief Register for Event
 *
 * Registers the provided Module for the Event with the provided name with a
 * callback for when the Event is triggered
 *
 * @param name         The name of the Event in which to register
 * @param parentModule The name of the owning Module (can be empty if created
 *   by the framework, default = "")
 * @param callback     A function pointer to a function that accepts the Event
 *   name and optional data for processing
 * @param priority     The priority of the registration (ascending priority,
 *   default = 0)
 *
 * @return true if the Event was found and registration succeeded, false
 *   otherwise
 */
bool EventHandling::registerForEvent(const std::string& name,
    const std::string& parentModule, void (*callback)(const std::string&,
    void*), const int& priority) {
  bool status = false;
  // Make sure the Event exists, and if specified, the Module exists
  if (EventHandling::events.count(name) > 0 &&
      (parentModule.length() == 0 ||
        ModuleManagement::getModuleByName(parentModule).get() != nullptr)) {
    // Add the requested EventRegistration to the Event
    EventHandling::events[name]->addRegistration(priority,
      std::shared_ptr<EventRegistration>{new EventRegistration{
        parentModule,
        callback
      }}
    );
    if (parentModule.length() > 0) Logger::debug("Module \"" + parentModule
      + "\" registered [R] for Event \"" + name + "\"");
    status = true;
  }
  return status;
}

/**
 * @brief Register Preprocessor for Event
 *
 * Registers the provided Module for the Event with the provided name with a
 * callback for before the Event is triggered to determine if it should be
 * triggered
 *
 * @remarks
 *   The provided callback should return true if the Event trigger can
 *   proceed, false otherwise
 *
 * @param name         The name of the Event in which to register
 * @param parentModule The name of the owning Module (can be empty if created
 *   by the framework, default = "")
 * @param callback     A function pointer to a function that accepts the Event
 *   name
 * @param priority     The priority of the registration (ascending priority,
 *   default = 0)
 *
 * @return true if the Event was found and registration succeeded, false
 *   otherwise
 */
bool EventHandling::registerPreprocessorForEvent(const std::string& name,
    const std::string& parentModule, bool (*callback)(const std::string&),
    const int& priority) {
  bool status = false;
  // Make sure the Event exists, and if specified, the Module exists
  if (EventHandling::events.count(name) > 0 &&
      (parentModule.length() == 0 ||
        ModuleManagement::getModuleByName(parentModule).get() != nullptr)) {
    // Add the requested EventRegistration to the Event
    EventHandling::events[name]->addPreprocessor(priority,
      std::shared_ptr<EventPreprocessor>{new EventPreprocessor{
        parentModule,
        callback
      }}
    );
    if (parentModule.length() > 0) Logger::debug("Module \"" + parentModule
      + "\" registered [P] for Event \"" + name + "\"");
    status = true;
  }
  return status;
}

/**
 * @brief Trigger Event
 *
 * Triggers the Event with the provided name and optional provided data
 *
 * @param name The name of the Event to be triggered
 * @param data The optional data to be included to each registration's
 *   callback
 *
 * @return true if the Event was found and triggered, false otherwise
 */
bool EventHandling::triggerEvent(const std::string& name, void* data) {
  bool status = false;
  if (EventHandling::events.count(name) > 0) {
    Logger::debug("Triggering Event \"" + name + "\" ...");
    EventHandling::events[name]->trigger(data);
    status = true;
  }
  return status;
}

/**
 * @brief Unregister Events
 *
 * Destroys the Events associated with the provided Module, along with all
 * of their associated internal data
 *
 * @param parentModule The name of the owning Module
 *
 * @return true if the Events were found and destroyed, false otherwise
 */
bool EventHandling::unregisterEvents(const std::string& parentModule) {
  Logger::debug("Deleting Event(s) owned by Module \""
    + parentModule + "\"");
  bool status = false;
  for (auto& event : EventHandling::events)
    if (event.second->getParentModule() == parentModule)
      status = EventHandling::destroyEvent(event.second->getName()) || status;
  return status;
}

/**
 * @brief Unregister for Event
 *
 * Unregisters the provided Module from the provided Event
 *
 * @param name         The name of the Event in which to unregister from
 * @param parentModule The name of the owning Module
 *
 * @return true if the Event was found and Module unregistered, false
 *   otherwise
 */
bool EventHandling::unregisterForEvent(const std::string& name,
    const std::string& parentModule) {
  bool status = false;
  if (EventHandling::events.count(name) > 0) {
    // Call delRegistration for the given Event
    EventHandling::events[name]->delRegistration(parentModule);
    if (parentModule.length() > 0) Logger::debug("Module \"" + parentModule
      + "\" unregistered [R] for Event \"" + name + "\"");
    status = true;
  }
  return status;
}

/**
 * @brief Unregister Preprocessor for Event
 *
 * Unregisters the provided Module from the provided Event
 *
 * @param name         The name of the Event in which to unregister from
 * @param parentModule The name of the owning Module
 *
 * @return true if the Event was found and Module unregistered, false
 *   otherwise
 */
bool EventHandling::unregisterPreprocessorForEvent(const std::string& name,
    const std::string& parentModule) {
  bool status = false;
  if (EventHandling::events.count(name) > 0) {
    // Call delPreprocessor for the given Event
    EventHandling::events[name]->delPreprocessor(parentModule);
    if (parentModule.length() > 0) Logger::debug("Module \"" + parentModule
      + "\" unregistered [P] for Event \"" + name + "\"");
    status = true;
  }
  return status;
}

/**
 * @brief Unregister Module
 *
 * Unregisters the provided Module from all Events
 *
 * @param parentModule The name of the owning Module
 *
 * @return true if the Events were found and Module unregistered, false
 *   otherwise
 */
bool EventHandling::unregisterModule(const std::string& parentModule) {
  bool status = false;
  for (auto& event : EventHandling::events) {
    status = EventHandling::unregisterForEvent(event.second->getName(),
      parentModule) || status;
    status = EventHandling::unregisterPreprocessorForEvent(
      event.second->getName(), parentModule) || status;
    if (parentModule.length() > 0) Logger::debug("Module \"" + parentModule
      + "\" unregistered for all Events");
  }
  return status;
}
