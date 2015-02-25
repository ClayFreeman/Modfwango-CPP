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
    static std::map<std::string, Event> events;
    // Prevent this class from being instantiated
    EventHandling() {}
  public:
    static bool createEvent(std::string name, Module* module,
      void (*callback)(std::string, void*) = nullptr);
    static bool destroyEvent(std::string name);
    static bool registerForEvent(std::string name, Module* module,
      void (*callback)(std::string, void*));
    static bool registerPreprocessorForEvent(std::string name, Module* module,
      bool (*callback)(std::string));
    static bool triggerEvent(std::string name, void* data = nullptr);
    static bool unregisterEvents(Module* module);
    static bool unregisterForEvent(std::string name, Module* module);
    static bool unregisterPreprocessorForEvent(std::string name,
      Module* module);
    static bool unregisterModule(Module* module);
};

#endif
