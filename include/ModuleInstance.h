/**
* @file    ModuleInstance.h
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
#include "include/Module.h"

template<class T>
void delete_dlobject(void* p) {
  if (DEBUG == 1) std::cout << "DEBUG: dlclose(p)\n";
  dlclose(static_cast<T*>(p));
}

class ModuleInstance {
  public:
    // Declare storage for Module
    std::shared_ptr<Module> module;
    // Declare storage for the dlopen() object
    std::unique_ptr<void, void(*)(void*)> object;
    // Define destructor to correctly destroy elements
    ~ModuleInstance() {
      if (DEBUG == 1) std::cout << "DEBUG: Default ~ModuleInstance() ...\n";
      if (DEBUG == 1) std::cout << "DEBUG: Deleting Module ...\n";
      module.reset();
      if (DEBUG == 1) std::cout << "DEBUG: Deleting object ...\n";
      object.reset();
    }
};

#endif
