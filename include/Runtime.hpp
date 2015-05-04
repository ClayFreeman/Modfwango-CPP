/**
 * @file  Runtime.h
 * @brief Runtime
 *
 * Class definition for Runtime
 *
 * @author     Clay Freeman
 * @date       March 13, 2015
 */

#ifndef _RUNTIME_H
#define _RUNTIME_H

#include <map>
#include <string>

class Runtime {
  private:
    static std::map<std::string, std::string> options;
    // Prevent this class from being instantiated
    Runtime() {}
  public:
    static const std::string null_str;
    static bool add(const std::string& key, const std::string& value);
    static const std::string& get(const std::string& key);
};

#endif
