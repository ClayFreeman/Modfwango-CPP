/**
 * @file  Utility.hpp
 * @brief Utility
 *
 * Class definition for Utility
 *
 * @author     Clay Freeman
 * @date       March 20, 2015
 */

#ifndef _UTILITY_HPP
#define _UTILITY_HPP

#include <string>
#include <vector>

class Utility {
  private:
    // Prevent this class from being instantiated
    Utility() {}
  public:
    static std::vector<std::string> explode(const std::string& s,
      const std::string& d);
    static std::string& ltrim(std::string& s);
    static std::string& rtrim(std::string& s);
    static std::string& trim(std::string& s);
};

#endif
