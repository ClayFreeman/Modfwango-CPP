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
  if (DEBUG == 1) std::cout << "DEBUG: Start of main()\n";
  if (DEBUG == 1) std::cout << "DEBUG: Begin ModuleManagement tests ...\n";
  try {
    // Attempt to load Module Test.so in the modules directory
    ModuleManagement::loadModule("src/modules/Test.so");
  }
  catch (std::logic_error e) {
    // Something went wrong with the Module; tell the developer
    std::cout << e.what() << '\n';
  }
  catch (std::runtime_error e) {
    // Something went wrong loading the shared object
    std::cout << e.what() << '\n';
  }
  if (DEBUG == 1) std::cout << "DEBUG: End ModuleManagement tests ...\n";
  if (DEBUG == 1) std::cout << "DEBUG: End of main()\n";
  return 0;
}
