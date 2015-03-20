/**
 * @file  main.cpp
 * @brief Modfwango
 *
 * A test driver for the Modfwango-C++ framework
 *
 * @author     Clay Freeman
 * @date       January 23, 2015
 */

#include <fstream>
#include <iostream>
#include <limits.h>
#include <libgen.h>
#include <signal.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "include/ConnectionManagement.hpp"
#include "include/EventHandling.hpp"
#include "include/File.hpp"
#include "include/Logger.hpp"
#include "include/ModuleManagement.hpp"
#include "include/Runtime.hpp"
#include "include/SocketManagement.hpp"
#include "include/Utility.hpp"

void signal_handler(int signal);

int main(int argc, char* const argv[]) {
  // Setup the global __PROJECTROOT__ variable (safely)
  if (argc > 0) {
    // Allocate storage for realpath(...)
    char* tmp1 = (char*)calloc(PATH_MAX, sizeof(char));
    realpath(argv[0], tmp1);
    // Create a pointer for the static C-String provided by dirname(...)
    char* tmp2 = dirname(tmp1);
    // Create a copy of tmp2
    char* tmp3 = (char*)calloc(strlen(tmp2) + 1, sizeof(char));
    memcpy((char*)tmp3, tmp2, strlen(tmp2));
    // Free the first temporary variable
    free(tmp1);
    // Store the path in the Runtime class
    if (!Runtime::add("__PROJECTROOT__", tmp3)) {
      // Free the third temporary variable
      free(tmp3);
      throw std::logic_error{
        "Could not store __PROJECTROOT__ in Runtime instance"
      };
    }
    // Free the third temporary variable
    free(tmp3);
  }

  // Prepare environment
  Runtime::add("__MODFWANGOVERSION__", "1.00");
  Runtime::add("__STARTTIME__", std::to_string(time(nullptr)));
  if (argc > 1) Logger::setMode(atoi(argv[1]));
  else Logger::setMode(LOG_ALL);

  // Welcome via console
  Logger::info("Welcome to Modfwango!");
  Logger::info("You're running Modfwango v" +
    Runtime::get("__MODFWANGOVERSION__"));

  // Create directories/files
  mkdir((Runtime::get("__PROJECTROOT__") + "/conf").c_str(),
    S_IRWXU | S_IRWXG | S_IRWXO);
  File::create(Runtime::get("__PROJECTROOT__") + "/conf/listen.conf");
  File::create(Runtime::get("__PROJECTROOT__") + "/conf/modules.conf");

  // Load Modules
  for (auto module : Utility::explode(File::getContent(
      Runtime::get("__PROJECTROOT__") + "/conf/modules.conf"), "\n")) {
    if (module.length() > 0)
      ModuleManagement::loadModule(module);
  }

  // Load Sockets
  for (auto socket : Utility::explode(File::getContent(
      Runtime::get("__PROJECTROOT__") + "/conf/listen.conf"), "\n")) {
    if (socket.length() > 0 && socket.find(":") != std::string::npos) {
      // We don't support SSL yet ... :(
      if (socket.find("+") != std::string::npos)
        socket.erase(socket.find("+"), 1);
      // Create the Socket
      SocketManagement::newSocket(socket.substr(0, socket.find(":")),
        atoi(socket.substr(socket.find(":") + 1).c_str()));
    }
  }

  // Register signal_handler(...) as a callback for SIGINT
  signal(SIGINT, signal_handler);

  // Loop while there are Connections or Sockets still active and __DIE__ has
  // not been set
  while ((ConnectionManagement::count() > 0 || SocketManagement::count() > 0) &&
      Runtime::get("__DIE__").length() == 0) {
    // Stall until there is something to do on a Socket or Connection
    SocketManagement::stall();
    // Accept any incoming clients (if existent)
    SocketManagement::acceptConnections();
    // Prune any closed Connections
    ConnectionManagement::pruneConnections();
    // Loop through all active Connections ...
    for (auto i : ConnectionManagement::getConnections()) {
      try {
        // and read data in order to ...
        const std::string& data = i->getData();
        if (data.length() > 0)
          // pass each line of data to EventHandling
          for (auto d : Utility::explode(data, "\n"))
            EventHandling::receiveData(i, d);
      }
      catch (const std::runtime_error& e) {
        Logger::debug(e.what());
      }
    }
  }

  return 0;
}

/**
 * @brief Signal Handler
 *
 * Callback for signal handling
 *
 * @param signal The signal that was received
 */
void signal_handler(int signal) {
  // Mask ^C character
  std::cout << "\b\b";
  // Set the __DIE__ constant to trigger a shutdown
  Runtime::add("__DIE__", std::to_string(signal));
}
