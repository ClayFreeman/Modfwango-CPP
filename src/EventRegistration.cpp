/**
 * @file  EventRegistration.cpp
 * @brief EventRegistration
 *
 * Class implementation for EventRegistration
 *
 * @author     Clay Freeman
 * @date       February 27, 2015
 */

#include <iostream>
#include <string>
#include "../include/EventRegistration.h"
#include "../include/Logger.h"

/**
 * @brief Constructor
 *
 * Prepares the EventRegistration class with the provided arguments
 *
 * @param parentModule The name of the parent Module
 * @param callback     Pointer to the callback function
 */
EventRegistration::EventRegistration(std::string parentModule,
    void (*callback)(std::string, void*)) {
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
 */
void EventRegistration::call(const std::string& name, void* data) const {
  if (this->callback != nullptr)
    this->callback(name, data);
}
