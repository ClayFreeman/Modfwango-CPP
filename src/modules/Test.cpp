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
#include <sstream>
#include <string>
#include "../../include/EventHandling.h"
#include "../../include/Logger.h"
#include "../../include/Module.h"

class Test : public Module {
  public:
    // Initialize the name property
    Test() { this->setName("Test"); }
    // Demonstrate overriding the destructor
    ~Test() {}
    // Overload the isInstantiated() method
    bool isInstantiated();
    // Create Event callbacks
    static bool test_preprocessor(std::string name);
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
  Logger::stack(__FUNCTION__);
  Logger::debug("My name is: " + this->getName());
  EventHandling::createEvent("testEvent", this->getName());
  EventHandling::registerPreprocessorForEvent("testEvent", this->getName(),
    &Test::test_preprocessor);
  EventHandling::registerForEvent("testEvent", this->getName(),
    &Test::test_func);
  EventHandling::triggerEvent("testEvent", (void*)"Hello");
  Logger::stack(__FUNCTION__, true);
  return true;
}

/**
 * @brief Test Preprocessor
 *
 * A test Event callback preprocessor
 *
 * @param      name The name of the originating Event
 */
bool Test::test_preprocessor(std::string name) {
  Logger::stack(__FUNCTION__);
  Logger::debug("Preprocessing Event \"" + name + "\" ...");
  Logger::stack(__FUNCTION__, true);
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
  Logger::stack(__FUNCTION__);
  std::ostringstream oss;
  oss << data;
  Logger::devel("data: " + oss.str());
  Logger::debug("Event \"" + name + "\" callback: (const char*)\""
    + std::string{(const char*)data} + "\"");
  Logger::stack(__FUNCTION__, true);
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
