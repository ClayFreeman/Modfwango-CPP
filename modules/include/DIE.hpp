/**
 * @file  DIE.h
 * @brief DIE
 *
 * Class definition for DIE
 *
 * @author     Clay Freeman
 * @date       March 14, 2015
 */

#ifndef _DIE_H
#define _DIE_H

#include <string>
#include "../../include/Module.hpp"

class DIE : public Module {
  public:
    // Initialize the name property
    DIE() { this->setName("DIE"); }
    // Demonstrate overriding the destructor
    ~DIE() {}
    // Overload the isInstantiated() method
    bool isInstantiated();
    // Callback for RawEvent
    static void receiveRaw(const std::string& name, void* data);
};

#endif
