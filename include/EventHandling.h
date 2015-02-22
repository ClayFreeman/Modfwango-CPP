/**
 * @file  EventHandling.h
 * @brief EventHandling
 *
 * Class definition for EventHandling
 *
 * @author     Clay Freeman
 * @date       February 19, 2015
 */

#ifndef _EVENTHANDLING_H
#define _EVENTHANDLING_H

#include <map>
#include <string>
#include "Event.h"
#include "Module.h"

class EventHandling {
  private:
    std::map<std::string, Event> events{};
  public:
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
    bool createEvent(std::string name, Module* module,
      void (*callback)(std::string, void*) = nullptr);

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
    bool destroyEvent(std::string name);

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
    bool registerForEvent(std::string name, Module* module,
      void (*callback)(std::string, void*));

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
    bool registerPreprocessorForEvent(std::string name, Module* module,
      bool (*callback)(std::string));

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
    bool triggerEvent(std::string name, void* data = nullptr);

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
    bool unregisterEvents(Module* module);

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
    bool unregisterForEvent(std::string name, Module* module);

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
    bool unregisterPreprocessorForEvent(std::string name, Module* module);

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
    bool unregisterModule(Module* module);
};

#endif
