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
#include "../include/Event.h"
#include "../include/EventHandling.h"
#include "../include/EventRegistration.h"
#include "../include/Module.h"
#include "../include/ModuleManagement.h"

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
    const std::string& parentModule, void (*callback)(std::string, void*)) {
  bool status = false;
  // Prevent duplicate event names and make sure the parentModule exists (if
  // specified)
  if (name.length() > 0 && EventHandling::events.count(name) == 0 &&
      (parentModule.length() == 0 ||
        ModuleManagement::getModuleByName(parentModule).get() != nullptr)) {
    if (DEBUG == 1) std::cout << "DEBUG: Creating Event \"" << name <<
      "\" ...\n";
    // Create and insert the Event into the events map
    EventHandling::events[name] = std::shared_ptr<Event>{
      new Event{name, parentModule, callback}
    };
    // Return a true status upon creation
    status = true;
  }
  else if (DEBUG == 1) std::cout << "DEBUG: Problem creating Event \"" << name
    << "\"\n";
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
  if (DEBUG == 1) std::cout << "DEBUG: Destroying Event \"" << name <<
    "\" (manually) ...\n";
  return EventHandling::events.erase(name) > 0;
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
    const std::string& parentModule, void (*callback)(std::string, void*),
    const int& priority) {
  bool status = false;
  // Make sure the Event exists, and if specified, the Module exists
  if (EventHandling::events.count(name) > 0 &&
      (parentModule.length() == 0 ||
        ModuleManagement::getModuleByName(parentModule).get() != nullptr)) {
    if (DEBUG == 1 && parentModule.length() > 0) std::cout
      << "DEBUG: Registering Module \"" << parentModule << "\" for Event \""
      << name << "\" ...\n";
    else if (DEBUG == 1) std::cout << "DEBUG: Registering for Event \"" << name
      << "\" ...\n";
    // Add the requested EventRegistration to the Event
    EventHandling::events[name]->addRegistration(priority,
      std::shared_ptr<EventRegistration>{new EventRegistration{
        parentModule,
        callback
      }}
    );
    status = true;
  }
  else if (DEBUG == 1 && parentModule.length() > 0) std::cout
    << "DEBUG: Problem registering for Event \"" << name << "\" with Module \""
    << parentModule << "\"\n";
  else if (DEBUG == 1) std::cout << "DEBUG: Problem registering for Event \""
    << name << "\"\n";
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
    const std::string& parentModule, bool (*callback)(std::string),
    const int& priority) {
  return false;
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
  return false;
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
  return false;
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
  return false;
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
  return false;
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
  return false;
}
