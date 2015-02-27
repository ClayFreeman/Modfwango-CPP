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

class EventRegistration {
  private:
    std::string parentModule{};
    void (*callback)(std::string, void*) = nullptr;
    // Make sure copying is disallowed
    EventRegistration(const EventRegistration&);
    EventRegistration& operator= (const EventRegistration&);
  public:
    EventRegistration(std::string parentModule,
        void (*callback)(std::string, void*) = nullptr) {
      this->parentModule = parentModule;
      this->callback = callback;
    }
    const std::string& getParentModule() const { return parentModule; }
};

#endif
