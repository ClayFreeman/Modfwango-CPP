/**
 * @file  Logger.cpp
 * @brief Logger
 *
 * Class implementation for Logger
 *
 * @author     Clay Freeman
 * @date       March 3, 2015
 */

#include <iostream>
#include <string>
#include <vector>
#include "../ext/Utility/Utility.hpp"
#include "../include/Logger.hpp"

short Logger::mode = (DEBUG == 1 ? LOGLEVEL_DEVEL : LOGLEVEL_SILENT);
short Logger::indent = 0;

/**
 * @brief Debug
 *
 * Prints a debug message if debug mode is active
 *
 * @param msg The message to print
 */
void Logger::debug(const std::string& msg) {
  for (auto m : Utility::explode(msg, "\n"))
    if (m.length() > 0 && Logger::getMode() & LOG_DEBUG) {
      std::cout << COLOR_DEBUG << " DEBUG " << COLOR_RESET;
      for (int i = 0; i < Logger::indent; i++)
        std::cout << "  ";
      std::cout << "| " << m << '\n';
    }
}

/**
 * @brief Devel
 *
 * Prints a devel message if devel mode is active
 *
 * @param msg The message to print
 */
void Logger::devel(const std::string& msg) {
  for (auto m : Utility::explode(msg, "\n"))
    if (m.length() > 0 && Logger::getMode() & LOG_DEVEL) {
      std::cout << COLOR_DEVEL << " DEVEL " << COLOR_RESET;
      for (int i = 0; i < Logger::indent; i++)
        std::cout << "  ";
      std::cout << "| " << m << '\n';
    }
}

/**
 * @brief Get Mode
 *
 * Returns the current mode of Logger
 *
 * @return The mode
 */
short Logger::getMode() {
  return Logger::mode;
}

/**
 * @brief Info
 *
 * Prints an info message if info mode is active
 *
 * @param msg The message to print
 */
void Logger::info(const std::string& msg) {
  for (auto m : Utility::explode(msg, "\n"))
    if (m.length() > 0 && Logger::getMode() & LOG_INFO) {
      std::cout << COLOR_INFO << "  INFO " << COLOR_RESET;
      for (int i = 0; i < Logger::indent; i++)
        std::cout << "  ";
      std::cout << "| " << m << '\n';
    }
}

/**
 * @brief Set Mode
 *
 * Sets the mode for Logger
 *
 * @param m The mode to set
 *
 * @return true if valid, false otherwise
 */
bool Logger::setMode(short m) {
  if (m >= LOGLEVEL_SILENT && m <= LOGLEVEL_DEVEL) {
    Logger::devel("Setting log level to include:");
    if (m & LOG_INFO)  Logger::devel(" * LOG_INFO");
    if (m & LOG_STACK) Logger::devel(" * LOG_STACK");
    if (m & LOG_DEBUG) Logger::devel(" * LOG_DEBUG");
    if (m & LOG_DEVEL) Logger::devel(" * LOG_DEVEL");
  }
  return (m >= LOGLEVEL_SILENT && m <= LOGLEVEL_DEVEL) &&
    (Logger::mode = m) >= LOGLEVEL_SILENT;
}

/**
 * @brief Stack
 *
 * Prints a stack message if stack mode is active
 *
 * @param msg The message to print
 */
void Logger::stack(const std::string& func, bool end) {
  if (func.length() > 0 && Logger::getMode() & LOG_STACK) {
    if (end == true && Logger::indent > 0) Logger::indent--;
    std::cout << COLOR_STACK << " STACK " << COLOR_RESET;
    for (int i = 0; i < Logger::indent; i++)
      std::cout << "  ";
    std::cout << (end == true ? "- " : "+ ") << func << ";\n";
    if (end == false) Logger::indent++;
  }
}
