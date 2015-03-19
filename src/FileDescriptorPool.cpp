/**
 * @file  FileDescriptorPool.cpp
 * @brief FileDescriptorPool
 *
 * Class implementation for FileDescriptorPool
 *
 * @author     Clay Freeman
 * @date       March 13, 2015
 */

#include "../include/FileDescriptorPool.hpp"

// Initialize fds and max to 0
fd_set FileDescriptorPool::fds{{0}};
int    FileDescriptorPool::nfds{0};

/**
 * @brief Add
 *
 * Adds a raw file descriptor to the pool
 *
 * @param fd The file descriptor
 */
void FileDescriptorPool::add(int fd) {
  if (fd >= 0 && !FD_ISSET(fd, &FileDescriptorPool::fds)) {
    FD_SET(fd, &FileDescriptorPool::fds);
    if (FileDescriptorPool::nfds <= fd)
      FileDescriptorPool::nfds = fd + 1;
  }
}

/**
 * @brief Clear
 *
 * Clears the pool of file descriptors
 */
void FileDescriptorPool::clr() {
  FD_ZERO(&FileDescriptorPool::fds);
}

/**
 * @brief Delete
 *
 * Deletes a raw file descriptor from the pool
 *
 * @param parameter1 description of parameter1
 */
void FileDescriptorPool::del(int fd) {
  if (FD_ISSET(fd, &FileDescriptorPool::fds))
    FD_CLR(fd, &FileDescriptorPool::fds);
}

/**
 * @brief Get
 *
 * Returns the pool of file descriptors as a fd_set
 *
 * @return an fd_set of all descriptors
 */
fd_set FileDescriptorPool::get() {
  return FileDescriptorPool::fds;
}

/**
 * @brief Max
 *
 * Returns the maximum file descriptor (for usage with select(...))
 *
 * @return the max file descriptor
 */
int FileDescriptorPool::max() {
  return FileDescriptorPool::nfds;
}
