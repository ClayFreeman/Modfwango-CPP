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
#include <memory>
#include <string>
#include "Event.h"

class EventHandling {
  private:
    static std::map<std::string, std::shared_ptr<Event>> events;
    // Prevent this class from being instantiated
    EventHandling() {}
  public:
    static bool createEvent(const std::string& name,
      const std::string& parentModule = "",
      void (*callback)(std::string, void*) = nullptr);
    static bool destroyEvent(const std::string& name);
    static bool registerForEvent(const std::string& name,
      const std::string& parentModule, void (*callback)(std::string, void*),
      const int& priority = 0);
    static bool registerPreprocessorForEvent(const std::string& name,
      const std::string& parentModule, bool (*callback)(std::string),
      const int& priority = 0);
    static bool triggerEvent(const std::string& name, void* data = nullptr);
    static bool unregisterEvents(const std::string& parentModule);
    static bool unregisterForEvent(const std::string& name,
      const std::string& parentModule);
    static bool unregisterPreprocessorForEvent(const std::string& name,
      const std::string& parentModule);
    static bool unregisterModule(const std::string& parentModule);
};

#endif
