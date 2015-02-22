/**
 * @file  ModuleManagement.h
 * @brief ModuleManagement
 *
 * Class definition for ModuleManagement
 *
 * @author  Clay Freeman
 * @date    January 23, 2015
 */

#ifndef _MODULEMANAGEMENT_H
#define _MODULEMANAGEMENT_H

#include <memory>
#include <vector>
#include "include/Module.h"
#include "include/ModuleInstance.h"

class ModuleManagement {
  private:
    // Declare storage for loaded modules
    static std::vector<std::shared_ptr<ModuleInstance>> modules;
    // Prevent this class from being instantiated
    ModuleManagement() {}
    static std::string getBasename(std::string name);
  public:
    static std::shared_ptr<Module> getModuleByName(std::string name);
    static bool loadModule(std::string name);
    static bool reloadModule(std::string name);
    static bool unloadModule(std::string name);
};

#endif
