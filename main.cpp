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

  /**
   *   I'd like to look for a better way of doing this; An ideal solution would
   * be finding a way to make the destructor call for ModuleManagement, but it
   * is a static class.  The only other option I see is making it an instance
   * class, but I would rather not have it instantiable
   */
  ModuleManagement::unloadModule("Test");

  return 0;
}
