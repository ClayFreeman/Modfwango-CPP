/**
 * @file  ModuleInstance.cpp
 * @brief ModuleInstance
 *
 * Class implementation for ModuleInstance
 *
 * @author     Clay Freeman
 * @date       February 27, 2015
 */

#include <iostream>
#include <memory>
#include "../include/Logger.h"
#include "../include/Module.h"
#include "../include/ModuleInstance.h"

/**
 * @brief Constructor
 *
 * Constructs a ModuleInstance with a Module and its associated shared object
 *
 * @param parameter1 description of parameter1
 * @param parameter2 parameter2 is an output parameter.
 */
ModuleInstance::ModuleInstance(const std::shared_ptr<Module>& m,
  const std::shared_ptr<void>& o): module{m}, object{o} {}

/**
 * @brief Destructor
 *
 * Cleans up a Module and its shared object
 */
ModuleInstance::~ModuleInstance() {
  module.reset();
  object.reset();
}
