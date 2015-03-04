/**
 * @file  <class>.cpp
 * @brief <class>
 *
 * Class implementation for <class>
 *
 * @author     Clay Freeman
 * @date       March 3, 2015
 */

#include <iostream>
#include "../include/Logger.h"

short Logger::mode = (DEBUG == 1 ? LOG_ALL : LOG_INFO);

void Logger::debug(const std::string& msg) {
  if (Logger::getMode() & LOG_DEBUG) std::cout << COLOR_DEBUG << " DEBUG "
    << COLOR_RESET << "| " << msg << '\n';
}

void Logger::devel(const std::string& msg) {
  if (Logger::getMode() & LOG_DEVEL) std::cout << COLOR_DEVEL << " DEVEL "
    << COLOR_RESET << "| " << msg << '\n';
}

short Logger::getMode() {
  return Logger::mode;
}

void Logger::info(const std::string& msg) {
  if (Logger::getMode() & LOG_INFO) std::cout << COLOR_INFO << "  INFO "
    << COLOR_RESET << "| " << msg << '\n';
}

bool Logger::setMode(short m) {
  return (m >= LOG_SILENT && m <= LOG_ALL) && (Logger::mode = m) >= LOG_SILENT;
}

void Logger::stack(const std::string& func, bool end) {
  if (Logger::getMode() & LOG_STACK) std::cout << COLOR_STACK << " STACK "
    << COLOR_RESET << '|' << (end == true ? "-" : "+") << func << " ( ... );\n";
}
