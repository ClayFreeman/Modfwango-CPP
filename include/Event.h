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
        void (*dataCallback)(std::string, void*) = nullptr) {
      // Initialize the Event with the provided data
      this->name = name;
      this->parentModule = parentModule;
      this->dataCallback = dataCallback;
    }
    void addRegistration(const int& priority,
        const std::shared_ptr<EventRegistration>& registration) {
      // Add this EventRegistration for the provided priority to the end of
      // the vector
      this->registrations[priority].push_back(registration);
    }
    std::vector<std::shared_ptr<EventRegistration>> collapseRegistrations() {
      // Allocate storage for a return value
      std::vector<std::shared_ptr<EventRegistration>> retVal;
      for (auto entry = this->registrations.begin();
          entry != this->registrations.end(); ++entry)
        // Concatenate each vector to the return value in order
        retVal.insert(retVal.end(), entry->second.begin(), entry->second.end());
      return retVal;
    }
    void delRegistration(const std::string& parentModule) {
      for (auto& entry : this->registrations) {
        // Iterate through each vector in the map of EventRegistrations
        for (auto subentry = entry.second.begin();
            subentry != entry.second.end(); subentry++) {
          // Iterate through each EventRegistration in the vectors
          if ((*subentry)->getParentModule() == parentModule) {
            // If we get a match, erase the EventRegistration from the vector;
            // Using subentry-- because when you erase, all future iterators are
            // invalidated
            entry.second.erase(subentry--);
          }
        }
      }
    }
    const std::string& getName() const { return this->name; }
    const std::string& getParentModule() const { return this->parentModule; }
    void trigger(void* data) const {
      for (auto i : this->registrations)
        for (auto j : i.second)
          j->call(this->name, data);
    }
};

#endif
