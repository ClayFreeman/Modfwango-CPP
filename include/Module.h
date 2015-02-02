/**
 * @file    Module.h
 *
 * Class definition for a generic Module
 *
 * @author  Clay Freeman
 * @date    January 23, 2015
 */

#ifndef _MODULE_H
#define _MODULE_H

#include <iostream>
#include <string>

class Module {
  private:
    // Make sure copying is disallowed
    Module(const Module&);
    Module& operator= (const Module&);
  protected:
    // Each Module needs a name property
    std::string name = "";
  public:
    Module() {
      if (DEBUG == 1) std::cout << "DEBUG: Default Module()\n";
    }
    virtual ~Module() {
      if (DEBUG == 1) std::cout << "DEBUG: Default ~Module()\n";
    }
    // Each Module needs a getter for its name
    std::string getName() { return this->name; }
    // Each Module needs an isInstantiated method
    virtual bool isInstantiated() { return true; }
};

#endif
