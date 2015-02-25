/**
 * @file  ModuleManagement.cpp
 * @brief ModuleManagement
 *
 * Method implementations for ModuleManagement
 *
 * @author  Clay Freeman
 * @date    January 23, 2015
 */

#include <cstring>
#include <dlfcn.h>
#include <iostream>
#include <libgen.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "../include/Module.h"
#include "../include/ModuleInstance.h"
#include "../include/ModuleManagement.h"

// Initialize the modules vector
std::vector<std::shared_ptr<ModuleInstance>> ModuleManagement::modules{};

/**
 * @brief Get Basename
 *
 * Fetches the basename of the provided path
 *
 * @remarks
 * This method discards the ".so" file extension
 *
 * @param name The path or name
 *
 * @return The basename
 */
std::string ModuleManagement::getBasename(std::string name) {
  // Allocate a C-String for the basename operation
  char* n = new char[name.length() + 1];
  n[name.length()] = 0;

  // Copy the C-String from name into our C-String
  for (int i = 0; i < static_cast<int>(name.length()); i++) {
    n[i] = name.c_str()[i];
  }

  // Fetch the basename() of n
  char* base = basename(n);
  // Remove .so extension
  if (strncmp((base + (strlen(base) - 3)), ".so", 3) == 0) {
    *(base + (strlen(base) - 3)) = 0;
  }
  std::string ret = base;
  delete[] n;

  if (DEBUG == 1) std::cout << "DEBUG: Basename computed: \"" << ret << "\"\n";

  return ret;
}

/**
 * @brief Get Module by Name
 *
 * Fetches a Module (if it exists) by the associated name
 *
 * @remarks
 * If the Module doesn't exist, nullptr is returned
 *
 * @param name The name of the Module
 *
 * @return A pointer to the Module
 */
std::shared_ptr<Module> ModuleManagement::getModuleByName(std::string name) {
  for (std::shared_ptr<ModuleInstance> m : ModuleManagement::modules) {
    if (m->module->getName() == name) {
      if (DEBUG == 1) std::cout << "DEBUG: Fetched Module \"" << name << "\"\n";
      return m->module;
    }
  }
  return nullptr;
}

/**
 * @brief Load Module
 *
 *   Dynamically loads a shared object, fetches an instance of the Module within
 * it, and attempts to initialize it.  Upon success, it is added to the internal
 * vector of loaded modules
 *
 * @remarks
 * Will throw either a std::runtime_error or std::logic_error on failure
 *
 * @param name A path to the shared object
 *
 * @return true on success, false otherwise
 */
bool ModuleManagement::loadModule(std::string name) {
  if (DEBUG == 1) std::cout << "DEBUG: Loading Module \"" << name << "\" ...\n";
  std::string e;
  // Attempt to load the requested shared object
  void* obj = dlopen(name.c_str(), RTLD_NOW);
  if (obj != NULL) {
    // Clear any errors before continuing
    dlerror();
    // Fetch a pointer to the _load function
    Module* (*m)() = (Module* (*)())dlsym(obj, "_load");

    const char* err = dlerror();
    if (err == NULL) {
      // Fetch an instance of the module
      Module* module = m();
      if (module != nullptr &&
          module->getName() == ModuleManagement::getBasename(name)) {
        // Verify that the module can be loaded
        bool load = module->isInstantiated();
        if (load == true) {
          ModuleManagement::modules.push_back(std::shared_ptr<ModuleInstance>{
            new ModuleInstance {
              std::shared_ptr<Module>{module},
              std::shared_ptr<void>{obj, &delete_dlobject<void>}
            }
          });
          return true;
        }
        else {
          delete module;
          dlclose(obj);

          e += "Got false from ";
          e += ModuleManagement::getBasename(name);
          e += "->isInstantiated()";
          throw std::logic_error(e);
        }
      }
      else {
        if (module != nullptr) delete module;
        dlclose(obj);

        e += "Internal logic error in module \"";
        e += ModuleManagement::getBasename(name);
        e += "\" during _load()";
        throw std::logic_error(e);
      }
    }
    else {
      dlclose(obj);

      // Handle errors fetching a pointer
      e += err;
      throw std::runtime_error(e);
    }
  }
  else {
    // Handle miscellaneous errors
    const char* err = dlerror();
    e += err;
    throw std::runtime_error(e);
  }
  return false;
}

/**
 * @brief Reload Module
 *
 * Attempts to reload a Module
 *
 * @param name A path to the shared object
 *
 * @return true on success, false otherwise
 */
bool ModuleManagement::reloadModule(std::string name) {
  if (DEBUG == 1) std::cout << "DEBUG: Reloading Module \"" << name
    << "\" ...\n";
  // Unload the module, then load it again
  return ModuleManagement::unloadModule(ModuleManagement::getBasename(name)) &&
    ModuleManagement::loadModule(name);
}

/**
 * @brief Unload Module
 *
 * Unloads the requested Module
 *
 * @param name The name of the Module to unload
 *
 * @return true on success, false otherwise
 */
bool ModuleManagement::unloadModule(std::string name) {
  if (DEBUG == 1) std::cout << "DEBUG: Unloading Module \"" << name
    << "\" ...\n";
  // Iterate over each Module
  for (std::vector<std::shared_ptr<ModuleInstance>>::iterator i =
       ModuleManagement::modules.begin(); i !=
       ModuleManagement::modules.end(); ++i) {
    if ((*i)->module->getName() == name) {
      // Remove the ModuleInstance from the modules vector
      ModuleManagement::modules.erase(i);
      return true;
    }
  }
  return false;
}
