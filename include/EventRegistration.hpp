/**
 * @file  EventRegistration.h
 * @brief EventRegistration
 *
 * Class definition for EventRegistration
 *
 * @author     Clay Freeman
 * @date       February 24, 2015
 */

#ifndef _EVENTREGISTRATION_H
#define _EVENTREGISTRATION_H

#include <string>

class EventRegistration {
  private:
    std::string parentModule{};
    void (*callback)(const std::string&, void*) = nullptr;
    // Make sure copying is disallowed
    EventRegistration(const EventRegistration&);
    EventRegistration& operator= (const EventRegistration&);
  public:
    EventRegistration(const std::string& parentModule,
      void (*callback)(const std::string&, void*) = nullptr);
    const std::string& getParentModule() const;
    void call(const std::string& name, void* data) const;
};

#endif
