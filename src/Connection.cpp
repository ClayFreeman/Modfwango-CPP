/**
 * @file  Connection.cpp
 * @brief Connection
 *
 * Class implementation for Connection
 *
 * @author     Clay Freeman
 * @date       March 12, 2015
 */

#include <ctype.h>
#include <errno.h>
#include <locale>
#include <memory>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "../ext/Utility/Utility.hpp"
#include "../include/Connection.hpp"
#include "../include/FileDescriptor.hpp"
#include "../include/Logger.hpp"

/**
 * @brief Destructor
 *
 * Disconnects the Connection and destroys it
 */
Connection::~Connection() {
  this->reset();
}

/**
 * @brief Get Data
 *
 * Attempts to read data from the Connection (if available)
 *
 * @return The data that was read
 */
std::string Connection::getData() {
  // Prepare storage for the return value
  std::string retVal;

  // Make sure the socket is valid (open)
  if (this->isValid()) {
    // Prepare a file descriptor set in order to determine if there is data to
    // read from the socket
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(*this->sockfd, &rfds);
    struct timeval timeout{0, 0};
    // Use select(...) with a timeout of 0 to immediately determine if there is
    // data to read
    select(*this->sockfd + 1, &rfds, nullptr, nullptr, &timeout);

    // If the file descriptor is set in the set, there is data to read
    if (FD_ISSET(*this->sockfd, &rfds)) {
      // Prepare a buffer for the incoming data
      char* buffer = (char*)calloc(8192, sizeof(char));
      // Read up to (8K - 1) bytes from the file descriptor to ensure a null
      // character at the end to prevent overflow
      ssize_t count = read(*this->sockfd, buffer, 8191);
      // Copy the C-String into a std::string
      retVal = buffer;
      // Free the storage for the buffer ...
      free(buffer);
      // and trim the std::string
      Utility::trim(retVal);

      // If there was 0 bytes of data to read ...
      if (count < 1) {
        // this->sockfd marked readable, but no data was read; connection closed
        this->reset();
        throw std::runtime_error{"Connection reset by peer " + this->host +
          ":" + std::to_string(this->port)};
      }
    }
  }

  // Return a copy of the data that was read
  return retVal;
}

/**
 * @brief Get Host
 *
 * Returns the Connection's IP address
 *
 * @return A reference to the Connection's host
 */
const std::string& Connection::getHost() const {
  return this->host;
}

/**
 * @brief Get Port
 *
 * Returns the port number on which the Connection was received
 *
 * @return The Connection's port
 */
int Connection::getPort() const {
  return this->port;
}

/**
 * @brief Get Socket
 *
 * Fetches the file descriptor for the associated Connection
 *
 * @return A pointer to the Connection's file descriptor
 */
std::shared_ptr<FileDescriptor> Connection::getSock() const {
  return this->sockfd;
}

/**
 * @brief Is Valid
 *
 * Checks if the Connection is valid for operation (open)
 *
 * @return true if valid, false otherwise
 */
bool Connection::isValid() const {
  bool retVal = false;
  if (this->sockfd != nullptr)
    retVal = fcntl(*this->sockfd, F_GETFD) != -1 || errno != EBADF;
  return retVal;
}

/**
 * @brief Reset
 *
 * If the socket is valid, closes the socket
 */
void Connection::reset() {
  if (this->isValid()) {
    Logger::debug("Connection " + this->host + ":" +
      std::to_string(this->port) + " closed");
    this->sockfd.reset();
  }
}

/**
 * @brief Send
 *
 * Sends the provided data to the Connection's socket
 *
 * @param data The data to send
 */
void Connection::send(const std::string& data) const {
  if (this->isValid()) write(*this->sockfd, data.c_str(), data.length());
}
