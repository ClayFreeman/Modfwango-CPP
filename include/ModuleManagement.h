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

#include <map>
#include <memory>
#include <string>
#include "Module.h"
#include "ModuleInstance.h"

class ModuleManagement {
  private:
    // Declare storage for loaded modules
    static std::map<std::string, std::shared_ptr<ModuleInstance>> modules;
    // Prevent this class from being instantiated
    ModuleManagement() {}
    static std::string getBasename(const std::string& name);
  public:
    static std::shared_ptr<Module> getModuleByName(const std::string& name);
    static bool loadModule(const std::string& name);
    static bool reloadModule(const std::string& name);
    static bool unloadModule(const std::string& name);
};

#endif
