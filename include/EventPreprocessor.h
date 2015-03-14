/**
 * @file  EventPreprocessor.h
 * @brief EventPreprocessor
 *
 * Class definition for EventPreprocessor
 *
 * @author     Clay Freeman
 * @date       February 24, 2015
 */

#ifndef _EVENTPREPROCESSOR_H
#define _EVENTPREPROCESSOR_H

#include <string>

class EventPreprocessor {
  private:
    std::string parentModule{};
    bool (*callback)(std::string) = nullptr;
    // Make sure copying is disallowed
    EventPreprocessor(const EventPreprocessor&);
    EventPreprocessor& operator= (const EventPreprocessor&);
  public:
    EventPreprocessor(std::string parentModule,
      bool (*callback)(std::string) = nullptr);
    const std::string& getParentModule() const;
    bool call(const std::string& name) const;
};

#endif
