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
#include "../include/FileDescriptor.h"
#include "../include/FileDescriptorPool.h"

FileDescriptor::FileDescriptor(int fdi) {
  *this = fdi;
}

FileDescriptor::~FileDescriptor() {
  this->reset();
}

int FileDescriptor::get() const {
  return this->fd;
}

bool FileDescriptor::isValid() const {
  return fcntl(this->fd, F_GETFD) != -1 || errno != EBADF;
}

void FileDescriptor::reset() {
  if (this->isValid()) {
    FileDescriptorPool::del(this->fd);
    close(this->fd);
  }
}

FileDescriptor& FileDescriptor::operator=(int fdi) {
  this->reset();
  this->fd = fdi;
  if (this->isValid()) FileDescriptorPool::add(this->fd);
  return *this;
}

FileDescriptor::operator int() const {
  return this->fd;
}
