/**
 * @file  Test.cpp
 * @brief Test
 *
 * A test Module for the Modfwango-C++ framework
 *
 * @author     Clay Freeman
 * @date       January 23, 2015
 */

#include <iostream>
#include "../../include/Module.h"

class Test : public Module {
  public:
    // Initialize the name property
    Test() { this->setName("Test"); }
    // Demonstrate overriding the destructor
    ~Test() { if (DEBUG == 1) std::cout << "DEBUG: ~Test()\n"; }
    // Overload the isInstantiated() method
    bool isInstantiated();
};

/**
 * @brief Is Instantiated
 *
 * The method called directly after instantiation of this Module. This method is
 * used by the Module to prepare for loading
 *
 * @return true if loadable, false otherwise
 */
bool Test::isInstantiated() {
  std::cout << "Hello, World!\n";
  return true;
}

/**
 * @brief Load
 *
 * Makes the Module available through dlsym()
 *
 * @remarks
 * The memory for this Module must be freed when unloaded
 *
 * @return A pointer to this Module
 */
extern "C" Module* _load() { return new Test; }
