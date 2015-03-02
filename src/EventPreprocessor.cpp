/**
 * @file  EventPreprocessor.cpp
 * @brief EventPreprocessor
 *
 * Class implementation for EventPreprocessor
 *
 * @author     Clay Freeman
 * @date       February 27, 2015
 */

#include <iostream>
#include <string>
#include "../include/EventPreprocessor.h"

/**
 * @brief Constructor
 *
 * Prepares the EventPreprocessor class with the provided arguments
 *
 * @param parentModule The name of the parent Module
 * @param callback     Pointer to the callback function
 */
EventPreprocessor::EventPreprocessor(std::string parentModule,
    bool (*callback)(std::string)) {
  this->parentModule = parentModule;
  this->callback = callback;
}

/**
 * @brief Call
 *
 * Calls the internal callback pointer with the provided Event name and data
 *
 * @param name The name of the Event
 * @param data A pointer to some data
 *
 * @return the bool value returned by the callee
 */
bool EventPreprocessor::call(const std::string& name) const {
  if (DEBUG == 1) {
    std::cout << "DEBUG: Calling callback function for Event \"" << name
              << "\"";
    if (this->parentModule.length() > 0)
      std::cout << " for Module \"" << this->parentModule << "\"";
    std::cout << " ...\n";
  }
  return (this->callback != nullptr ? this->callback(name) : false);
}
