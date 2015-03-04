/**
 * @file  main.cpp
 * @brief Modfwango
 *
 * A test driver for the Modfwango-C++ framework
 *
 * @author     Clay Freeman
 * @date       January 23, 2015
 */

#include <cctype>
#include <iostream>
#include <stdexcept>
#include "include/Logger.h"
#include "include/ModuleManagement.h"

int main(int argc, char* argv[]) {
  if (argc > 1) Logger::setMode(atoi(argv[1]));
  try {
    // Attempt to load Module Test.so in the modules directory
    ModuleManagement::loadModule("src/modules/Test.so");
  }
  catch (std::logic_error e) {
    // Something went wrong with the Module; tell the developer
  }
  catch (std::runtime_error e) {
    // Something went wrong loading the shared object
  }
  return 0;
}
