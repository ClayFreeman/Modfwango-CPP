/**
 * @file  Module.h
 * @brief Module
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
    // Each Module needs a name property
    std::string name = "";
    // Make sure copying is disallowed
    Module(const Module&);
    Module& operator= (const Module&);
  protected:
    void setName(const std::string& name);
  public:
    Module() {}
    virtual ~Module() {}
    // Each Module needs a getter for its name
    const std::string& getName() const { return this->name; }
    // Each Module needs an isInstantiated method
    virtual bool isInstantiated() { return true; }
};

#endif
