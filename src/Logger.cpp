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
short Logger::indent = 0;

/**
 * @brief Debug
 *
 * Prints a debug message if debug mode is active
 *
 * @param msg The message to print
 */
void Logger::debug(const std::string& msg) {
  if (msg.length() > 0 && Logger::getMode() & LOG_DEBUG) {
    std::cout << COLOR_DEBUG << " DEBUG " << COLOR_RESET;
    for (int i = 0; i < Logger::indent; i++)
      std::cout << "  ";
    std::cout << "| " << msg << '\n';
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
  if (msg.length() > 0 && Logger::getMode() & LOG_DEVEL) {
    std::cout << COLOR_DEVEL << " DEVEL " << COLOR_RESET;
    for (int i = 0; i < Logger::indent; i++)
      std::cout << "  ";
    std::cout << "| " << msg << '\n';
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
  if (msg.length() > 0 && Logger::getMode() & LOG_INFO) {
    std::cout << COLOR_INFO << "  INFO " << COLOR_RESET;
    for (int i = 0; i < Logger::indent; i++)
      std::cout << "  ";
    std::cout << "| " << msg << '\n';
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
  return (m >= LOG_SILENT && m <= LOG_ALL) && (Logger::mode = m) >= LOG_SILENT;
}

/**
 * @brief Stack
 *
 * Prints a stack message if stack mode is active
 *
 * @param msg The message to print
 */
void Logger::stack(const std::string& func, bool end) {
  if (end == false && Logger::indent > 0) Logger::indent--
  if (func.length() > 0 && Logger::getMode() & LOG_STACK) {
    std::cout << COLOR_STACK << " STACK " << COLOR_RESET;
    for (int i = 0; i < Logger::indent; i++)
      std::cout << "  ";
    std::cout << (end == true ? "- " : "+ ") << func << ";\n";
  }
  if (end == true) Logger::indent++
}
