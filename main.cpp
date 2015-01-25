/**
 * @file  main.cpp
 * @brief Modfwango
 *
 * A test driver for the Modfwango-C++ framework
 *
 * @author     Clay Freeman
 * @date       January 23, 2015
 */

#include <iostream>
#include <stdexcept>
#include "include/ModuleManagement.h"

int main() {
  try {
    // Attempt to load Module Test.so in the modules directory
    ModuleManagement::loadModule("modules/Test.so");
  }
  catch (std::logic_error e) {
    // Something went wrong with the Module; tell the developer
    std::cout << e.what() << '\n';
  }
  catch (std::runtime_error e) {
    // Something went wrong loading the shared object
    std::cout << e.what() << '\n';
  }

  return 0;
}
