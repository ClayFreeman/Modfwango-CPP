/**
 * @file  FileDescriptor.cpp
 * @brief FileDescriptor
 *
 * Class implementation for FileDescriptor
 *
 * @author     Clay Freeman
 * @date       March 13, 2015
 */

#include <fcntl.h>
#include <errno.h>
#include "../include/FileDescriptor.hpp"
#include "../include/FileDescriptorPool.hpp"

/**
 * @brief Constructor
 *
 * Constructs a FileDescriptor with an integer representing a file descriptor
 *
 * @param fdi The file descriptor
 */
FileDescriptor::FileDescriptor(int fdi) {
  *this = fdi;
}

/**
 * @brief Destructor
 *
 * Resets this FileDescriptor to a plain state
 */
FileDescriptor::~FileDescriptor() {
  this->reset();
}

/**
 * @brief Get
 *
 * Returns the raw file descriptor
 *
 * @return The raw file descriptor
 */
int FileDescriptor::get() const {
  return this->fd;
}

/**
 * @brief Is Valid
 *
 * Checks if the FileDescriptor is valid for operation (open)
 *
 * @return true if valid, false otherwise
 */
bool FileDescriptor::isValid() const {
  return fcntl(this->fd, F_GETFD) != -1 || errno != EBADF;
}

/**
 * @brief Reset
 *
 * Removes the FileDescriptor from the FileDescriptorPool and closes it
 */
void FileDescriptor::reset() {
  if (this->isValid()) {
    FileDescriptorPool::del(this->fd);
    close(this->fd);
  }
}

/**
 * @brief Assignment Operator Overload
 *
 * Allows assigning an integer to a FileDescriptor (alias for constructor)
 *
 * @param fdi The file descriptor
 *
 * @return A FileDescriptor reference
 */
FileDescriptor& FileDescriptor::operator=(int fdi) {
  this->reset();
  this->fd = fdi;
  if (this->isValid()) FileDescriptorPool::add(this->fd);
  return *this;
}

/**
 * @brief Conversion Operator Overload
 *
 * Allows implicit conversion of a FileDescriptor to an int
 *
 * @return The raw file descriptor
 */
FileDescriptor::operator int() const {
  return this->fd;
}
