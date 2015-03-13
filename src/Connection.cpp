/**
 * @file  Connection.cpp
 * @brief Connection
 *
 * Class implementation for Connection
 *
 * @author     Clay Freeman
 * @date       March 12, 2015
 */

#include <algorithm>
#include <ctype.h>
#include <errno.h>
#include <functional>
#include <locale>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "../include/Connection.h"
#include "../include/Logger.h"

Connection::~Connection() {
  this->reset();
}

std::string& Connection::ltrim(std::string& s) const {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(
    std::ptr_fun<int, int>(std::isspace))));
  return s;
}

std::string& Connection::rtrim(std::string& s) const {
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(
    std::isspace))).base(), s.end());
  return s;
}

std::string& Connection::trim(std::string& s) const {
  return this->ltrim(this->rtrim(s));
}

std::string Connection::getData() const {
  std::string retVal;

  if (this->isValid()) {
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(this->sockfd, &rfds);
    struct timeval timeout{0, 0};
    select(this->sockfd + 1, &rfds, nullptr, nullptr, &timeout);

    if (FD_ISSET(this->sockfd, &rfds)) {
      char* buffer = (char*)calloc(8192, sizeof(char));
      ssize_t count = read(this->sockfd, buffer, 8191);
      retVal = buffer;
      free(buffer);
      trim(retVal);

      if (count < 1) {
        // this->sockfd marked readable, but no data was read; connection closed
        this->reset();
        throw std::runtime_error{"Connection reset by peer " + this->host +
          ":" + std::to_string(this->port)};
      }
      else
        Logger::debug("Received data:\n" + retVal);
    }
  }

  return retVal;
}

void Connection::reset() const {
  if (this->isValid()) {
    Logger::debug("Connection " + this->host + ":" +
      std::to_string(this->port) + " closed");
    close(this->sockfd);
  }
}

void Connection::send(const std::string& data) const {
  if (this->isValid()) write(this->sockfd, data.c_str(), data.length());
}
