/**
 * @file  Runtime.cpp
 * @brief Runtime
 *
 * Class implementation for Runtime
 *
 * @author     Clay Freeman
 * @date       March 13, 2015
 */

#include <map>
#include <string>
#include "../include/Runtime.hpp"

std::map<std::string, std::string> Runtime::options{};

/**
 * @brief Add
 *
 * Sets the (permanent) value for the provided key
 *
 * @param key   The key
 * @param value The value
 *
 * @return true if added, false otherwise
 */
bool Runtime::add(const std::string& key, const std::string& value) {
  bool retVal = false;
  if (Runtime::options.count(key) == 0 || Runtime::options[key].length() == 0) {
    Runtime::options[key] = value;
    retVal = true;
  }
  return retVal;
}

/**
 * @brief Get
 *
 * Returns the value for the associated key
 *
 * @param key The key
 *
 * @return The value
 */
const std::string& Runtime::get(const std::string& key) {
  return Runtime::options[key];
}
