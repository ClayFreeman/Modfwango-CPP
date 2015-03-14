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
