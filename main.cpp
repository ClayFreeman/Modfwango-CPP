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
#include <stdexcept>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "include/ConnectionManagement.h"
#include "include/EventHandling.h"
#include "include/File.h"
#include "include/Logger.h"
#include "include/ModuleManagement.h"
#include "include/Runtime.h"
#include "include/SocketManagement.h"

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

  // Load modules
  std::string module;
  std::ifstream modules;
  modules.open(Runtime::get("__PROJECTROOT__") + "/conf/modules.conf");
  while (modules.good() && std::getline(modules, module)) {
    ModuleManagement::loadModule(module);
    modules.peek();
  }
  modules.close();

  // Load sockets
  std::string socket;
  std::ifstream sockets;
  sockets.open(Runtime::get("__PROJECTROOT__") + "/conf/listen.conf");
  while (sockets.good() && std::getline(sockets, socket)) {
    if (socket.find(":") != std::string::npos) {
      if (socket.find("+") != std::string::npos)
        socket.erase(socket.find("+"));
      SocketManagement::newSocket(socket.substr(0, socket.find(":")),
        atoi(socket.substr(socket.find(":") + 1).c_str()));
      sockets.peek();
    }
  }
  sockets.close();

  while ((ConnectionManagement::count() > 0 || SocketManagement::count() > 0) &&
      Runtime::get("__DIE__").length() == 0) {
    SocketManagement::stall();
    SocketManagement::acceptConnections();
    ConnectionManagement::pruneConnections();
    for (auto i : ConnectionManagement::getConnections()) {
      try {
        const std::string& data = i->getData();
        if (data.length() > 0) EventHandling::receiveData(i, data);
      }
      catch (const std::runtime_error&) {}
    }
  }

  return 0;
}
