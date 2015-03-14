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
#include "../include/Runtime.h"

std::map<std::string, std::string> Runtime::options{};

bool Runtime::add(const std::string& key, const std::string& value) {
  bool retVal = false;
  if (Runtime::options.count(key) == 0) {
    Runtime::options[key] = value;
    retVal = true;
  }
  return retVal;
}

const std::string& Runtime::get(const std::string& key) {
  return Runtime::options[key];
}
