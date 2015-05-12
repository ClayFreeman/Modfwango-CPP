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
#include "../include/Logger.hpp"
#include "../include/Module.hpp"
#include "../include/ModuleInstance.hpp"

/**
 * @brief Constructor
 *
 * Constructs a ModuleInstance with a Module and its associated shared object
 *
 * @param m std::shared_ptr to the instantiated Module
 * @param o std::shared_ptr to the handle returned by dlopen(...)
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
