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
int prepare_environment(int argc, const char* const argv[]);
void prepare_runtime();
void signal_handler(int signal);
void start_runtime(int loglevel);

int main(int argc, const char* const argv[]) {
  // Prepare runtime environment variables
  int loglevel = prepare_environment(argc, argv);

  // Prepare for runtime execution by loading modules and sockets
  prepare_runtime();

  // Start runtime with the requested log level
  start_runtime(loglevel);

  return 0;
}

/**
 * @brief Background
 *
 * Puts the current running process in the background as a daemon
 */
void background() {
  Logger::stack(__PRETTY_FUNCTION__);
  Logger::devel("Daemonizing ...");
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
  Logger::stack(__PRETTY_FUNCTION__, true);
}

/**
 * @brief Prepare Environment
 *
 * Declares Runtime constants required for operation, performs safety and
 * integrity checks, sets the working directory, determins a user-defined log
 * level, and creates required files
 *
 * @remarks
 *   In order for this function to finish, Modfwango must be used as a submodule
 *
 * @param argc Count of arguments
 * @param argv Array of C-String arguments
 *
 * @return Requested log level
 */
int prepare_environment(int argc, const char* const argv[]) {
  Logger::stack(__PRETTY_FUNCTION__);
  // Record the timestamp that Modfwango was started
  Logger::devel("Recording __STARTTIME__ using Unix epoch ...");
  Runtime::add("__STARTTIME__", std::to_string(time(nullptr)));

  Logger::devel("Attempting to find executable path from argv[0] ...");
  const std::string exe{File::realPath(argv[0])};
  if (argc > 0 && File::isFile(exe) && File::executable(exe)) {
    // Record the full path to the executable
    Logger::devel("Found executable path from argv[0]");
    Runtime::add("__EXECUTABLE__", exe);
  }
  else {
    Logger::info("Could not determine executable path from argv[0]");
    exit(-1);
  }

  // Declare the Modfwango root, which is the parent directory of the executable
  Logger::devel("Calculating __MODFWANGOROOT__ from __EXECUTABLE__ ...");
  Runtime::add("__MODFWANGOROOT__", File::directory(
    Runtime::get("__EXECUTABLE__")));

  // Declare the project root, which should always be the parent directory of
  // __MODFWANGOROOT__
  Logger::devel("Calculating __PROJECTROOT__ from __MODFWANGOROOT__ ...");
  Runtime::add("__PROJECTROOT__", File::directory(
    Runtime::get("__MODFWANGOROOT__")));

  // Exit if theoretical and actual __PROJECTROOT__ differ
  Logger::devel("Checking validity of __PROJECTROOT__ ...");
  const std::string theoretical_root = File::realPath(File::directory(argv[0]));
  if (Runtime::get("__PROJECTROOT__") != theoretical_root) {
    Logger::info("__PROJECTROOT__ does not match the expected directory \"" +
      theoretical_root + "\" - Is Modfwango being launched directly?");
    exit(-2);
  }

  // Exit if project & Modfwango roots match
  Logger::devel("Checking if __MODFWANGOROOT__ and __PROJECTROOT__ match ...");
  if (Runtime::get("__MODFWANGOROOT__") == Runtime::get("__PROJECTROOT__")) {
    Logger::info("__MODFWANGOROOT__ and __PROJECTROOT__ cannot match (\"" +
      Runtime::get("__MODFWANGOROOT__") + "\")");
    exit(-3);
  }

  // Verify both project & Modfwango roots as safe
  Logger::devel("Checking safety of __PROJECTROOT__ and __MODFWANGOROOT__ ...");
  std::string path_regex{"^[a-zA-Z0-9\\/._-]+$"};
  if (!std::regex_match(Runtime::get("__PROJECTROOT__"),
      std::regex(path_regex))) {
    Logger::info("__PROJECTROOT__ (\"" + Runtime::get("__PROJECTROOT__") +
      "\") is not a safe path.  Root paths must match \"" + path_regex + "\"");
    exit(-4);
  }
  if (!std::regex_match(Runtime::get("__MODFWANGOROOT__"),
      std::regex(path_regex))) {
    Logger::info("__MODFWANGOROOT__ (\"" + Runtime::get("__MODFWANGOROOT__") +
      "\") is not a safe path.  Root paths must match \"" + path_regex + "\"");
    exit(-5);
  }

  // Change directory to the project root
  Logger::devel("Changing the working directory to __PROJECTROOT__ ...");
  chdir(Runtime::get("__PROJECTROOT__").c_str());

  // Fetch log level from either CLI or config file, otherwise use default
  Logger::devel("Attempt to determine user-defined log level ...");
  short loglevel = Logger::getMode();
  if (argc > 1) {
    short tmp = atoi(argv[1]);
    if (tmp >= 0 && tmp < LOGLEVELSIZE) {
      // If the requested log level is safe, use it
      Logger::devel("Using log level provided from command-line argument");
      loglevel = LogLevels[tmp];
    }
  }
  else {
    const std::string loglevel_conf{Runtime::get("__PROJECTROOT__") +
      "/conf/loglevel.conf"};
    if (File::isFile(loglevel_conf)) {
      short tmp = atoi(File::getContent(loglevel_conf).c_str());
      if (tmp >= 0 && tmp < LOGLEVELSIZE) {
        // If the requested log level is safe, use it
        Logger::devel("Using log level provided from configuration");
        loglevel = LogLevels[tmp];
      }
    }
  }

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
    Logger::devel("Creating directory at path \"" + i + "\" ...");
    mkdir((Runtime::get("__PROJECTROOT__") + i).c_str(),
      S_IRWXU | S_IRWXG | S_IRWXO);
    if (!File::isDirectory(Runtime::get("__PROJECTROOT__") + i)) {
      Logger::info("Error creating mandatory directory \"" +
        Runtime::get("__PROJECTROOT__") + i + "\"");
      exit(-6);
    }
  }
  for (auto i : files) {
    File::create(Runtime::get("__PROJECTROOT__") + i);
    if (!File::isFile(Runtime::get("__PROJECTROOT__") + i)) {
      Logger::info("Error creating mandatory file \"" +
        Runtime::get("__PROJECTROOT__") + i + "\"");
      exit(-7);
    }
  }

  // Check for early exit request
  if (argc > 1 && isalpha(*argv[1])) {
    std::string s(strlen(argv[1]), 0);
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
  Logger::devel("Project name set to \"" + Runtime::get("__NAME__") + "\"");

  #ifdef __linux__
  // Assign "process" title
  prctl(PR_SET_NAME, Runtime::get("__NAME__").c_str(), 0, 0, 0);
  #endif

  // Check for process conflicts
  const std::string pidfile = Runtime::get("__PROJECTROOT__") + "/data/" +
    Runtime::get("__NAME__") + ".pid";
  if (File::isFile(pidfile)) {
    Logger::devel("Found PID file \"" + pidfile + "\"");
    const int pid = atoi(File::getContent(pidfile).c_str());
    if (pid > 0 && (kill(pid, 0) == 0 || errno == EPERM)) {
      Logger::info("Modfwango is already running with PID " +
        std::to_string(pid));
      exit(-8);
    }
  }

  // Exit if unable to write PID to file
  File::create(pidfile);
  if (!File::putContent(pidfile, std::to_string(getpid()))) {
    Logger::info("Error writing PID file");
    exit(-9);
  }

  Logger::stack(__PRETTY_FUNCTION__, true);
  return loglevel;
}

/**
 * @brief Prepare Runtime
 *
 * Set __MODFWANGOVERSION__, brag, and load modules & sockets
 */
void prepare_runtime() {
  Logger::stack(__PRETTY_FUNCTION__);
  // TODO:  For now, set version explicitly until we have docs/CHANGELOG.md
  Runtime::add("__MODFWANGOVERSION__", "1.00");

  // Ensure loglevel is set to include LOG_INFO until backgrounded
  Logger::setMode(Logger::getMode() | LOG_INFO);

  // Greet the user
  Logger::info("Welcome to Modfwango!");
  Logger::info("You're running Modfwango v" +
    Runtime::get("__MODFWANGOVERSION__"));

  // Load Modules.
  for (auto root : { "__MODFWANGOROOT__", "__PROJECTROOT__" })
    for (auto module : Utility::explode(File::getContent(
        Runtime::get(root) + "/conf/modules.conf"), "\n"))
      if (module.length() > 0)
        ModuleManagement::loadModule(module);

  // Load Sockets
  for (auto socket : Utility::explode(File::getContent(
      Runtime::get("__PROJECTROOT__") + "/conf/listen.conf"), "\n")) {
    if (socket.length() > 0) {
      std::vector<std::string> v{Utility::explode(socket, ",")};
      // We don't support SSL yet ... :(
      if (v[1].substr(0, 1) == "+")
        v[1].erase(0, 1);
      // Create the Socket
      SocketManagement::newSocket(v[0], atoi(v[1].c_str()));
    }
  }
  Logger::stack(__PRETTY_FUNCTION__, true);
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

/**
 * @brief Start Runtime
 *
 * Backgrounds the process if necessary, or registers a signal handler.  Then,
 * begins the main loop until there are no more Sockets or Connections, or
 * __DIE__ has been set
 *
 * @param loglevel The log level to set before starting
 */
void start_runtime(int loglevel) {
  // Immediately set the log level
  Logger::setMode(loglevel);
  // Go into background if necessary
  if (Logger::getMode() == 0) background();
  // Otherwise, register a signal handler
  else signal(SIGINT, signal_handler);

  // Loop while there are Connections or Sockets still active and __DIE__ has
  // not been set
  while ((ConnectionManagement::count() > 0 ||
      SocketManagement::count() > 0) &&
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
            EventHandling::receiveData(i, Utility::trim(d));
      }
      catch (const std::runtime_error& e) {
        Logger::debug(e.what());
      }
    }
  }
}
