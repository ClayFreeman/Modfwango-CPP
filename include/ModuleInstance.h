/**
* @file    ModuleInstance.h
*
* Struct definition for ModuleInstance
*
* @author  Clay Freeman
* @date    January 24, 2015
*/

#ifndef _MODULEINSTANCE_H
#define _MODULEINSTANCE_H

#include "include/Module.h"

struct ModuleInstance {
  // Declare storage for Module
  Module* module;
  // Declare storage for the dlopen() object
  void* object;
};

#endif
