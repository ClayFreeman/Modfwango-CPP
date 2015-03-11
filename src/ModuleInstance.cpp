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
 * @brief Destructor
 *
 * Cleans up a Module and its shared object
 */
ModuleInstance::~ModuleInstance() {
  module.reset();
  object.reset();
}
