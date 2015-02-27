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
#include "../include/Module.h"
#include "../include/ModuleInstance.h"

/**
 * @brief Constructor
 *
 * Prepares the ModuleInstance class with the provided arguments
 *
 * @param m A pointer to the loaded Module
 * @param o A pointer to the dlopen() result
 */
ModuleInstance::ModuleInstance(const std::shared_ptr<Module>& m,
    const std::shared_ptr<void>& o) {
  this->module = m;
  this->object = o;
}

/**
 * @brief Destructor
 *
 * Cleans up a Module and its shared object
 */
ModuleInstance::~ModuleInstance() {
  if (DEBUG == 1) std::cout << "DEBUG: Default ~ModuleInstance() ...\n";
  if (DEBUG == 1) std::cout << "DEBUG: Deleting Module ...\n";
  module.reset();
  if (DEBUG == 1) std::cout << "DEBUG: Deleting object ...\n";
  object.reset();
}
