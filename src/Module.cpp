/**
 * @file  Module.cpp
 * @brief Module
 *
 * Class implementation for Module
 *
 * @author     Clay Freeman
 * @date       February 27, 2015
 */

#include <string>
#include "../include/Logger.h"
#include "../include/Module.h"

Module::Module() {}

Module::~Module() {}

// Each Module needs a getter for its name
const std::string& Module::getName() const {
  return this->name;
}

// Each Module needs an isInstantiated method
bool Module::isInstantiated() {
  return true;
}

/**
 * @brief Set Name
 *
 * Sets the Module name parameter
 *
 * @param name The name of the Module
 */
void Module::setName(const std::string& name) {
  // If the name wasn't set already ...
  if (this->name.length() == 0)
    // set the name to the provided value
    this->name = name;
}
