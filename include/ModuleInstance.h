/**
 * @file  ModuleInstance.h
 * @brief ModuleInstance
 *
 * Class definition for ModuleInstance
 *
 * @author  Clay Freeman
 * @date    January 24, 2015
 */

#ifndef _MODULEINSTANCE_H
#define _MODULEINSTANCE_H

#include <dlfcn.h>
#include <iostream>
#include <memory>
#include "Module.h"

template<class T>
void delete_dlobject(void* p) {
  if (DEBUG == 1) std::cout << "DEBUG: dlclose(p)\n";
  dlclose(static_cast<T*>(p));
}

class ModuleInstance {
  private:
    // Make sure copying is disallowed
    ModuleInstance(const ModuleInstance&);
    ModuleInstance& operator= (const ModuleInstance&);
  public:
    // Declare storage for Module
    std::shared_ptr<Module> module;
    // Declare storage for the dlopen() object
    std::shared_ptr<void> object;
    // Define default constructor to accept module and object
    ModuleInstance(const std::shared_ptr<Module>& m,
      const std::shared_ptr<void>& o);
    // Define destructor to correctly destroy elements
    ~ModuleInstance();
};

#endif
