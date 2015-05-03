/**
 * @file  main.cpp
 * @brief Modfwango
 *
 * A test driver for the Modfwango-C++ framework
 *
 * @author     Clay Freeman
 * @date       January 23, 2015
 */

#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <regex>
#include <signal.h>
#ifdef __linux__
#include <sys/prctl.h>
#endif
#include <sys/stat.h>
#include <unistd.h>
#include "include/ConnectionManagement.hpp"
#include "include/EventHandling.hpp"
#include "include/File.hpp"
#include "include/Logger.hpp"
#include "include/ModuleManagement.hpp"
#include "include/Runtime.hpp"
#include "include/SocketManagement.hpp"
#include "include/Utility.hpp"

// Declare helper function prototypes
void background();
void prepare_environment(int argc, char* const argv[]);
void prepare_runtime();
void start_runtime();

int main(int argc, char* const argv[]) {
  // Welcome the user
  Logger::info("Welcome to Modfwango!");

  // Prepare runtime environment variables
  prepare_environment(argc, argv);

  // Sleep for 10 seconds for testing
  sleep(10);

  // // Load configuration
  // load_config();
  //
  // // Background the current process if in daemon mode
  // if (Logger::getMode() == LOGLEVEL_SILENT) background();
  //
  // // Run main loop
  // loop();

  return 0;
}

/**
 * @brief Background
 *
 * Puts the current running process in the background as a daemon
 */
void background() {
  // Fork and terminate parent process
  if (fork() > 0) _exit(0);
  // Create a new session
  setsid();
  // Fork and terminate parent process
  if (fork() > 0) _exit(0);
  // Set the user mask to zero
  umask(0);

  // Open standard fds to /dev/null
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  open("/dev/null", O_RDONLY);
  open("/dev/null", O_WRONLY);
  open("/dev/null", O_RDWR);
}

// void load_config() {
//   // Load Modules
//   for (auto module : Utility::explode(File::getContent(
//       Runtime::get("__PROJECTROOT__") + "/conf/modules.conf"), "\n")) {
//     if (module.length() > 0)
//       ModuleManagement::loadModule(module);
//   }
//
//   // Load Sockets
//   for (auto socket : Utility::explode(File::getContent(
//       Runtime::get("__PROJECTROOT__") + "/conf/listen.conf"), "\n")) {
//     if (socket.length() > 0) {
//       std::vector<std::string> v{Utility::explode(socket, ":")};
//       // We don't support SSL yet ... :(
//       if (v[1].substr(0, 1) == "+")
//         v[1].erase(0, 1);
//       // Create the Socket
//       SocketManagement::newSocket(v[0], atoi(v[1].c_str()));
//     }
//   }
// }

// void loop() {
//   // Loop while there are Connections or Sockets still active and __DIE__ has
//   // not been set
//   while ((ConnectionManagement::count() > 0 || SocketManagement::count() > 0) &&
//       Runtime::get("__DIE__").length() == 0) {
//     // Stall until there is something to do on a Socket or Connection
//     SocketManagement::stall();
//     // Accept any incoming clients (if existent)
//     SocketManagement::acceptConnections();
//     // Prune any closed Connections
//     ConnectionManagement::pruneConnections();
//     // Loop through all active Connections ...
//     for (auto i : ConnectionManagement::getConnections()) {
//       try {
//         // and read data in order to ...
//         const std::string& data = i->getData();
//         if (data.length() > 0)
//           // pass each line of data to EventHandling
//           for (auto d : Utility::explode(data, "\n"))
//             EventHandling::receiveData(i, Utility::trim(d));
//       }
//       catch (const std::runtime_error& e) {
//         Logger::debug(e.what());
//       }
//     }
//   }
// }

void prepare_environment(int argc, char* const argv[]) {
  // Record the timestamp that Modfwango was started
  Runtime::add("__STARTTIME__", std::to_string(time(nullptr)));

  if (argc > 0 && File::isFile(File::realPath(argv[0])) &&
      File::executable(File::realPath(argv[0])))
    // Record the full path to the executable
    Runtime::add("__EXECUTABLE__", File::realPath(argv[0]));
  else {
    Logger::info("Could not determine executable path from argv[0]");
    exit(-1);
  }

  // Declare the Modfwango root, which is the parent directory of the executable
  Runtime::add("__MODFWANGOROOT__", File::directory(
    Runtime::get("__EXECUTABLE__")));

  // Declare the project root, which should always be the parent directory of
  // __MODFWANGOROOT__
  // NOTE:  Maybe verify this better in the future (via symlink)?
  Runtime::add("__PROJECTROOT__", File::directory(
    Runtime::get("__MODFWANGOROOT__")));

  // Exit if project & Modfwango roots match
  if (Runtime::get("__MODFWANGOROOT__") == Runtime::get("__PROJECTROOT__")) {
    Logger::info("__MODFWANGOROOT__ and __PROJECTROOT__ cannot match (\"" +
      Runtime::get("__MODFWANGOROOT__") + "\")");
    exit(-2);
  }

  // Verify both project & Modfwango roots as safe
  std::string path_regex{"^[a-zA-Z0-9\\/._-]+$"};
  if (!std::regex_match(Runtime::get("__PROJECTROOT__"),
      std::regex(path_regex))) {
    Logger::info("__PROJECTROOT__ (\"" + Runtime::get("__PROJECTROOT__") +
      "\") is not a safe path.  Root paths must match \"" + path_regex + "\"");
    exit(-3);
  }
  if (!std::regex_match(Runtime::get("__MODFWANGOROOT__"),
      std::regex(path_regex))) {
    Logger::info("__MODFWANGOROOT__ (\"" + Runtime::get("__MODFWANGOROOT__") +
      "\") is not a safe path.  Root paths must match \"" + path_regex + "\"");
    exit(-4);
  }

  // Change directory to the project root
  chdir(Runtime::get("__PROJECTROOT__").c_str());

  // Fetch loglevel from either CLI or config file
  short loglevel = -1;
  if (argc > 1)
    loglevel = atoi(argv[1]);
  else {
    const std::string loglevel_conf{Runtime::get("__PROJECTROOT__") +
      "/conf/loglevel.conf"};
    if (File::isFile(loglevel_conf))
      loglevel = atoi(File::getContent(loglevel_conf).c_str());
  }

  // Setup an array of possible log levels
  const int MODESIZE = 5;
  short modes[MODESIZE] = {
    LOGLEVEL_SILENT,
    LOGLEVEL_INFO,
    LOGLEVEL_STACK,
    LOGLEVEL_DEBUG,
    LOGLEVEL_DEVEL
  };

  // Set the requested log level (contrary to default if valid)
  if (loglevel >= 0 && loglevel < MODESIZE) Logger::setMode(modes[loglevel]);

  // Create mandatory directories/files
  const std::string dirs[] = {
    "/conf",
    "/data",
    "/modules"
  };
  const std::string files[] = {
    "/conf/listen.conf",
    "/conf/modules.conf"
  };
  for (auto i : dirs) {
    mkdir((Runtime::get("__PROJECTROOT__") + i).c_str(),
      S_IRWXU | S_IRWXG | S_IRWXO);
    if (!File::isDirectory(Runtime::get("__PROJECTROOT__") + i)) {
      Logger::info("Error creating mandatory directory \"" +
        Runtime::get("__PROJECTROOT__") + i + "\"");
      exit(-5);
    }
  }
  for (auto i : files) {
    File::create(Runtime::get("__PROJECTROOT__") + i);
    if (!File::isFile(Runtime::get("__PROJECTROOT__") + i)) {
      Logger::info("Error creating mandatory file \"" +
        Runtime::get("__PROJECTROOT__") + i + "\"");
      exit(-6);
    }
  }

  // Warn about non-recommended configuration(s)
  if (File::getContent(Runtime::get("__PROJECTROOT__") +
      "/conf/modules.conf").length() == 0)
    Logger::info("WARNING:  No modules will be loaded");

  // Check for early exit request
  if (argc > 1 && isalpha(*argv[1])) {
    std::string s;
    std::transform(argv[1], argv[1] + strlen(argv[1]), s.begin(), tolower);
    if (s == "prelaunch") {
      Logger::info("Exiting early ...");
      exit(0);
    }
  }

  // Load process name from file, or choose default "modfwango"
  if (File::isFile(Runtime::get("__PROJECTROOT__") + "/conf/name.conf")) {
    const std::string tmp{File::getContent(Runtime::get("__PROJECTROOT__") +
      "/conf/name.conf")};
    if (std::regex_match(tmp, std::regex("^[a-zA-Z]+[a-zA-Z0-9_-]*$")))
      Runtime::add("__NAME__", tmp);
  }
  Runtime::add("__NAME__", "modfwango");

  // Assign process title
  #ifdef __linux__
  prctl(PR_SET_NAME, Runtime::get("__NAME__").c_str(), 0, 0, 0);
  #endif

  // Check for process conflicts
  if (File::isFile(Runtime::get("__PROJECTROOT__") + "/data/" +
      Runtime::get("__NAME__") + ".pid")) {
    if (kill(atoi(File::getContent(Runtime::get("__PROJECTROOT__") + "/data/" +
        Runtime::get("__NAME__") + ".pid").c_str()), 0) == 0 ||
        errno == EPERM) {
      Logger::info("Modfwango is already running");
      exit(-7);
    }
  }
  if (!File::putContent(Runtime::get("__PROJECTROOT__") + "/data/" +
      Runtime::get("__NAME__") + ".pid", std::to_string(getpid()))) {
    Logger::info("Error writing PID file");
    exit(-8);
  }
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
