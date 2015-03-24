/**
 * @file  ModuleManagement.cpp
 * @brief ModuleManagement
 *
 * Method implementations for ModuleManagement
 *
 * @author  Clay Freeman
 * @date    January 23, 2015
 */

#include <dlfcn.h>
#include <libgen.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <string.h>
#include <vector>
#include "../include/Logger.hpp"
#include "../include/Module.hpp"
#include "../include/ModuleInstance.hpp"
#include "../include/ModuleManagement.hpp"

std::map<std::string, std::shared_ptr<ModuleInstance>>
  ModuleManagement::modules{};

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
std::string ModuleManagement::getBasename(const std::string& name) {
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
std::shared_ptr<Module> ModuleManagement::getModuleByName(
    const std::string& name) {
  if (ModuleManagement::modules.count(name) > 0) {
    return ModuleManagement::modules[name]->module;
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
bool ModuleManagement::loadModule(const std::string& name) {
  std::string path = "modules/src/" + name + ".so";
  bool status = false;
  Logger::debug("Attempting to load module at path \"" + path + "\" ...");
  std::string e;
  if (!ModuleManagement::getModuleByName(ModuleManagement::getBasename(path))) {
    // Attempt to load the requested shared object
    void* obj = dlopen(path.c_str(), RTLD_NOW);
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
            module->getName() == ModuleManagement::getBasename(path)) {
          // Add the module to the internal array
          ModuleManagement::modules[module->getName()] =
            std::shared_ptr<ModuleInstance>{
              new ModuleInstance {
                std::shared_ptr<Module>{module},
                std::shared_ptr<void>{obj, &delete_dlobject<void>}
              }
            };
          // Verify that the module can be loaded
          if (module->isInstantiated() == true) {
            // Module loaded successfully
            status = true;
            Logger::info("Loaded Module \"" + module->getName() + "\"");
          }
          else {
            Logger::debug("Unable to load module \""
              + ModuleManagement::getBasename(module->getName()) + "\"");

            e = "Module refused to load during \"" + module->getName() +
              "::isInstantiated()\"";
            Logger::debug(e);

            // Unload the module
            ModuleManagement::unloadModule(module->getName());

            throw std::logic_error(e);
          }
        }
        else {
          if (module != nullptr) delete module;
          dlclose(obj);

          Logger::debug("Unable to load module at path \"" + path + "\"");

          e = "Internal logic error in module at path \"" + path +
            "\" during _load()";
          Logger::debug(e);
          throw std::logic_error(e);
        }
      }
      else {
        dlclose(obj);

        Logger::debug("Unable to load module at path \"" + path + "\"");

        // Handle errors fetching a pointer
        e = err;
        Logger::debug(e);
        throw std::runtime_error(e);
      }
    }
    else {
      // Handle miscellaneous errors
      const char* err = dlerror();

      Logger::debug("Unable to load module at path \"" + path + "\"");

      e = err;
      Logger::debug(e);
      throw std::runtime_error(e);
    }
  }
  return status;
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
bool ModuleManagement::reloadModule(const std::string& name) {
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
bool ModuleManagement::unloadModule(const std::string& name) {
  if (ModuleManagement::modules.count(name) > 0)
    Logger::info("Unloaded Module \"" + name + "\" ...");
  return ModuleManagement::modules.erase(name) > 0;
}
