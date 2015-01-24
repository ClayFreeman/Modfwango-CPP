/**
 * @file    Module.cpp
 *
 * Method implementations for Module
 *
 * @author  Clay Freeman
 * @date    January 23, 2015
 */

#include <string>
#include "include/Module.h"

/**
 * @brief Get Name
 *
 * Fetches the value of protected property Module::name
 *
 * @return The name of this Module instance
 */
std::string Module::getName() {
  return this->name;
}
