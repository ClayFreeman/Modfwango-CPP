/**
 * @file    ModuleManagement.h
 *
 * Class definition for ModuleManagement
 *
 * @author  Clay Freeman
 * @date    January 23, 2015
 */

#ifndef _MODULEMANAGEMENT_H
#define _MODULEMANAGEMENT_H

#include <vector>
#include "include/Module.h"
#include "include/ModuleInstance.h"

class ModuleManagement {
  private:
    // Declare storage for loaded modules
    static std::vector<ModuleInstance*> modules;
    // Prevent this class from being instantiated
    ModuleManagement();
    static std::string getBasename(std::string name);
    static Module* getModuleByName(std::string name);

  public:
    ~ModuleManagement();
    static bool loadModule(std::string name);
    static bool reloadModule(std::string name);
    static bool unloadModule(std::string name);
};

#endif
