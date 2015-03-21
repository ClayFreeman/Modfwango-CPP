/**
 * @file  Utility.cpp
 * @brief Utility
 *
 * Class implementation for Utility
 *
 * @author     Clay Freeman
 * @date       March 20, 2015
 */

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include "../include/Utility.hpp"

/**
 * @brief Explode
 *
 * Explodes a std::string by a delimiter to a std::vector of std::string
 *
 * @param s The std::string to explode
 * @param d The delimiter to explode the std::string
 *
 * @return std::vector of std::string
 */
std::vector<std::string> Utility::explode(const std::string& s,
    const std::string& d) {
  std::size_t lpos = 0;
  std::vector<std::string> result;

  for (std::size_t cpos = 0; (cpos = s.find(d, lpos)) != std::string::npos;
      lpos = cpos + d.length())
    // Add each item separated by a delimiter
    result.push_back(s.substr(lpos, cpos - lpos));
  // Add the last substr with no delimiter
  result.push_back(s.substr(lpos));

  return result;
}

/**
 * @brief Left Trim
 *
 * Trims whitespace from the left side of the provided std::string
 *
 * @param[out] s The std::string to trim
 *
 * @return The modified std::string
 */
std::string& Utility::ltrim(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(
    std::ptr_fun<int, int>(std::isspace))));
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(
    std::ptr_fun<int, int>(std::iscntrl))));
  return s;
}

/**
 * @brief Right Trim
 *
 * Trims whitespace from the right side of the provided std::string
 *
 * @param[out] s The std::string to trim
 *
 * @return The modified std::string
 */
std::string& Utility::rtrim(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(
    std::isspace))).base(), s.end());
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(
    std::iscntrl))).base(), s.end());
  return s;
}

/**
 * @brief Trim
 *
 * Trims whitespace from the ends of the provided std::string
 *
 * @param[out] s The std::string to trim
 *
 * @return The modified std::string
 */
std::string& Utility::trim(std::string& s) {
  return Utility::ltrim(Utility::rtrim(s));
}
