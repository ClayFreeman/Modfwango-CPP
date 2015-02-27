/**
 * @file  Event.h
 * @brief Event
 *
 * Class definition for Event
 *
 * @author     Clay Freeman
 * @date       February 24, 2015
 */

#ifndef _EVENT_H
#define _EVENT_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "EventRegistration.h"

class Event {
  private:
    std::string name{};
    std::string parentModule{};
    std::map<int, std::vector<std::shared_ptr<EventRegistration>>>
      registrations{};
    void (*dataCallback)(std::string, void*) = nullptr;
    // Make sure copying is disallowed
    Event(const Event&);
    Event& operator= (const Event&);
  public:
    Event(const std::string& name, const std::string& parentModule,
      void (*dataCallback)(std::string, void*) = nullptr);
    void addRegistration(const int& priority,
      const std::shared_ptr<EventRegistration>& registration);
    void delRegistration(const std::string& parentModule);
    const std::string& getName() const { return this->name; }
    const std::string& getParentModule() const { return this->parentModule; }
    void trigger(void* data) const;
};

#endif
