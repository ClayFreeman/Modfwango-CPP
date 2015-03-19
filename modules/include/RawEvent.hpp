/**
 * @file  RawEvent.h
 * @brief RawEvent
 *
 * Class definition for RawEvent
 *
 * @author     Clay Freeman
 * @date       March 14, 2015
 */

#ifndef _RAWEVENT_H
#define _RAWEVENT_H

#include <memory>
#include <string>
#include "../../include/Connection.hpp"
#include "../../include/Module.hpp"

struct RawEventData {
  std::shared_ptr<Connection> c;
  std::string                 d;
};

class RawEvent : public Module {
  public:
    // Initialize the name property
    RawEvent() { this->setName("RawEvent"); }
    // Demonstrate overriding the destructor
    ~RawEvent() {}
    // Overload the isInstantiated() method
    bool isInstantiated();
    // Data callback for RawEvent
    static void receiveRaw(const std::string& name,
      std::shared_ptr<Connection> connection, std::string data);
};

#endif
