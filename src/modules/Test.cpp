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
#include <string>
#include "../../include/EventHandling.h"
#include "../../include/Module.h"

class Test : public Module {
  public:
    // Initialize the name property
    Test() { this->setName("Test"); }
    // Demonstrate overriding the destructor
    ~Test() { if (DEBUG == 1) std::cout << "DEBUG: ~Test()\n"; }
    // Overload the isInstantiated() method
    bool isInstantiated();
    // Create an Event callback
    static void test_func(std::string name, void* data);
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
  if (DEBUG == 1) std::cout << "DEBUG: My name: \"" << this->getName()
    << "\" ...\n";

  if (DEBUG == 1) std::cout << "DEBUG: Begin EventHandling tests ...\n";
  EventHandling::createEvent("TestManualDeallocationEvent");
  EventHandling::createEvent("TestScopeDeallocationEvent", this->getName());
  EventHandling::destroyEvent("TestManualDeallocationEvent");
  EventHandling::registerForEvent("TestScopeDeallocationEvent", this->getName(),
    &Test::test_func);
  if (DEBUG == 1) std::cout << "DEBUG: End EventHandling tests ...\n";
  return true;
}

/**
 * @brief Test Function
 *
 * A test Event callback function
 *
 * @param      name The name of the originating Event
 * @param[out] data The optional data given with the Event trigger
 */
void Test::test_func(std::string name, void* data) {
  if (DEBUG == 1) std::cout << "DEBUG: Event \"" << name
    << "\" callback received\n";
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
