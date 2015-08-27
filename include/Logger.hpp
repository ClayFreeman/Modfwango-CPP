/**
 * @file  Logger.h
 * @brief Logger
 *
 * Class definition for Logger
 *
 * @author     Clay Freeman
 * @date       March 3, 2015
 */

#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>

// Used internally for console color codes
#define COLOR_INFO  "\x1b[32;01m" // Green
#define COLOR_DEBUG "\x1b[33;01m" // Yellow
#define COLOR_DEVEL "\x1b[31;01m" // Red
#define COLOR_STACK "\x1b[36;01m" // Yellow
#define COLOR_RESET "\x1b[0m"     // Regular

// Add these numbers to combine modes
#define LOG_SILENT      0  // [0x0000] Silent execution
#define LOG_INFO        1  // [0x0001] Informational output
#define LOG_STACK       2  // [0x0010] Shows entrance and exit of functions
#define LOG_DEBUG       4  // [0x0100] Output related to code workflow
#define LOG_DEVEL       8  // [0x1000] Output for data/resource debugging

// Define ceiling log levels
#define LOGLEVEL_SILENT 0  // [0x0000] Show nothing
#define LOGLEVEL_INFO   1  // [0x0001] Show info
#define LOGLEVEL_STACK  3  // [0x0011] Show stack and info
#define LOGLEVEL_DEBUG  7  // [0x0111] Show debug, stack, and info
#define LOGLEVEL_DEVEL  15 // [0x1111] Show all output

// Setup an array of possible log levels
const int LOGLEVELSIZE  = 5;
const short LogLevels[LOGLEVELSIZE] = {
  LOGLEVEL_SILENT,
  LOGLEVEL_INFO,
  LOGLEVEL_STACK,
  LOGLEVEL_DEBUG,
  LOGLEVEL_DEVEL
};

class Logger {
  private:
    static short mode;
    static short indent;
    // Prevent this class from being instantiated
    Logger() {}
  public:
    static void  debug(const std::string& msg);
    static void  devel(const std::string& msg);
    static short getMode();
    static void  info(const std::string& msg);
    static bool  setMode(short m);
    static void  stack(const std::string& func, bool end = false);
};

#endif
