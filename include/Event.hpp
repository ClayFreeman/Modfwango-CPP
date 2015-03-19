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
#include "Connection.hpp"
#include "EventPreprocessor.hpp"
#include "EventRegistration.hpp"

class Event {
  private:
    std::string name{};
    std::string parentModule{};
    std::map<int, std::vector<std::shared_ptr<EventPreprocessor>>>
      preprocessors{};
    std::map<int, std::vector<std::shared_ptr<EventRegistration>>>
      registrations{};
    void (*dataCallback)(const std::string&, std::shared_ptr<Connection>,
      std::string) = nullptr;
    // Make sure copying is disallowed
    Event(const Event&);
    Event& operator= (const Event&);
  public:
    Event(const std::string& name, const std::string& parentModule,
      void (*dataCallback)(const std::string&, std::shared_ptr<Connection>,
      std::string) = nullptr);
    void addRegistration(const int& priority,
      const std::shared_ptr<EventRegistration>& registration);
    void addPreprocessor(const int& priority,
      const std::shared_ptr<EventPreprocessor>& preprocessor);
    void call(std::shared_ptr<Connection> c, const std::string& data) const;
    void delRegistration(const std::string& parentModule);
    void delPreprocessor(const std::string& parentModule);
    const inline std::string& getName() const { return this->name; }
    const inline std::string& getParentModule() const
      { return this->parentModule; }
    void trigger(void* data) const;
};

#endif
