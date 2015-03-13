/**
 * @file  Event.cpp
 * @brief Event
 *
 * Class implementation for Event
 *
 * @author     Clay Freeman
 * @date       February 27, 2015
 */

#include <ctype.h>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "../include/Event.h"
#include "../include/EventRegistration.h"
#include "../include/Logger.h"

/**
 * @brief Constructor
 *
 * Prepares the Event with the given arguments
 *
 * @param name         The name of the Event
 * @param parentModule The name of the parent Module
 * @param dataCallback A pointer to a method to handle data callbacks (optional)
 */
Event::Event(const std::string& name, const std::string& parentModule,
    void (*dataCallback)(std::string, void*)) {
  // Initialize the Event with the provided data
  this->name = name;
  this->parentModule = parentModule;
  this->dataCallback = dataCallback;
}

/**
 * @brief Add Registration
 *
 * Adds a registration at the given priority
 *
 * @param priority     The priority of the registration (ascending, default = 0)
 * @param registration The EventRegistration to add
 */
void Event::addRegistration(const int& priority,
    const std::shared_ptr<EventRegistration>& registration) {
  // Add this EventRegistration for the provided priority to the end of
  // the vector
  this->registrations[priority].push_back(registration);
}

/**
 * @brief Add Preprocessor
 *
 * Adds a preprocessor at the given priority
 *
 * @param priority     The priority of the preprocessor (ascending, default = 0)
 * @param registration The EventRegistration to add
 */
void Event::addPreprocessor(const int& priority,
    const std::shared_ptr<EventPreprocessor>& preprocessor) {
  // Add this EventPreprocessor for the provided priority to the end of
  // the vector
  this->preprocessors[priority].push_back(preprocessor);
}

/**
 * @brief Delete Registration
 *
 * Deletes the registration(s) for the given Module
 *
 * @param parentModule The name of the parent Module
 */
void Event::delRegistration(const std::string& parentModule) {
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

/**
 * @brief Delete Preprocessor
 *
 * Deletes the preprocessor(s) for the given Module
 *
 * @param parentModule The name of the parent Module
 */
void Event::delPreprocessor(const std::string& parentModule) {
  for (auto& entry : this->preprocessors) {
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

/**
 * @brief Trigger
 *
 * Triggers the Event with the specified data
 *
 * @param data A pointer to some optional data
 */
void Event::trigger(void* data) const {
  for (auto i : this->preprocessors)
    for (auto j : i.second)
      if (!j->call(this->name)) return;
  for (auto i : this->registrations)
    for (auto j : i.second)
      j->call(this->name, data);
}
