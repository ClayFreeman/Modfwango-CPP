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
#include <string>
#include "../include/Event.h"
#include "../include/EventHandling.h"
#include "../include/Module.h"

// Initialize the events map
std::map<std::string, Event> EventHandling::events{};

/**
 * @brief Create Event
 *
 * Creates an Event with the provided optional data callback and owned by
 * the provided Module
 *
 * @param name     The name of the Event to be created
 * @param module   A pointer to the owning Module (can be nullptr if created
 *   by the framework)
 * @param callback An optional function pointer to a function that accepts
 *   the Event name and data for processing
 *
 * @return true if the Event was created, false otherwise
 */
bool EventHandling::createEvent(std::string name, Module* module,
    void (*callback)(std::string, void*)) {
  //
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
bool EventHandling::destroyEvent(std::string name) {
  return EventHandling::events.erase(name) > 0;
}

/**
 * @brief Register for Event
 *
 * Registers the provided Module for the Event with the provided name with a
 * callback for when the Event is triggered
 *
 * @param name     The name of the Event in which to register
 * @param module   A pointer to the owning Module (can be nullptr if created
 *   by the framework)
 * @param callback A function pointer to a function that accepts the Event
 *   name and optional data for processing
 *
 * @return true if the Event was found and registration succeeded, false
 *   otherwise
 */
bool EventHandling::registerForEvent(std::string name, Module* module,
    void (*callback)(std::string, void*)) {
  //
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
 * @param name     The name of the Event in which to register
 * @param module   A pointer to the owning Module (can be nullptr if created
 *   by the framework)
 * @param callback A function pointer to a function that accepts the Event
 *   name
 *
 * @return true if the Event was found and registration succeeded, false
 *   otherwise
 */
bool EventHandling::registerPreprocessorForEvent(std::string name,
    Module* module, bool (*callback)(std::string)) {
  //
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
bool EventHandling::triggerEvent(std::string name, void* data) {
  //
}

/**
 * @brief Unregister Events
 *
 * Destroys the Events associated with the provided Module, along with all
 * of their associated internal data
 *
 * @param module A pointer to the owning Module
 *
 * @return true if the Events were found and destroyed, false otherwise
 */
bool EventHandling::unregisterEvents(Module* module) {
  //
}

/**
 * @brief Unregister for Event
 *
 * Unregisters the provided Module from the provided Event
 *
 * @param name   The name of the Event in which to unregister from
 * @param module A pointer to the owning Module
 *
 * @return true if the Event was found and Module unregistered, false
 *   otherwise
 */
bool EventHandling::unregisterForEvent(std::string name, Module* module) {
  //
}

/**
 * @brief Unregister Preprocessor for Event
 *
 * Unregisters the provided Module from the provided Event
 *
 * @param name   The name of the Event in which to unregister from
 * @param module A pointer to the owning Module
 *
 * @return true if the Event was found and Module unregistered, false
 *   otherwise
 */
bool EventHandling::unregisterPreprocessorForEvent(std::string name,
    Module* module) {
  //
}

/**
 * @brief Unregister Module
 *
 * Unregisters the provided Module from all Events
 *
 * @param module A pointer to the owning Module
 *
 * @return true if the Events were found and Module unregistered, false
 *   otherwise
 */
bool EventHandling::unregisterModule(Module* module) {
  //
}
