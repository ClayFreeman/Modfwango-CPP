/**
 * @file  FileDescriptorPool.cpp
 * @brief FileDescriptorPool
 *
 * Class implementation for FileDescriptorPool
 *
 * @author     Clay Freeman
 * @date       March 13, 2015
 */

#include "../include/FileDescriptorPool.h"

// Initialize fds and max to 0
fd_set FileDescriptorPool::fds{{0}};
int    FileDescriptorPool::nfds{0};

void FileDescriptorPool::add(int fd) {
  if (fd >= 0 && !FD_ISSET(fd, &FileDescriptorPool::fds)) {
    FD_SET(fd, &FileDescriptorPool::fds);
    if (FileDescriptorPool::nfds <= fd)
      FileDescriptorPool::nfds = fd + 1;
  }
}

void FileDescriptorPool::clr() {
  FD_ZERO(&FileDescriptorPool::fds);
}

void FileDescriptorPool::del(int fd) {
  if (FD_ISSET(fd, &FileDescriptorPool::fds))
    FD_CLR(fd, &FileDescriptorPool::fds);
}

fd_set FileDescriptorPool::get() {
  return FileDescriptorPool::fds;
}

int FileDescriptorPool::max() {
  return FileDescriptorPool::nfds;
}
