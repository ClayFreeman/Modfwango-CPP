/**
 * @file  FileDescriptorPool.h
 * @brief FileDescriptorPool
 *
 * Class definition for FileDescriptorPool
 *
 * @author     Clay Freeman
 * @date       March 13, 2015
 */

#ifndef _FILEDESCRIPTORPOOL_H
#define _FILEDESCRIPTORPOOL_H

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

class FileDescriptorPool {
  private:
    static fd_set fds;
    static int    nfds;
    // Prevent this class from being instantiated
    FileDescriptorPool() {}
  public:
    static void   add(int fd);
    static void   clr();
    static void   del(int fd);
    static fd_set get();
    static int    max();
};

#endif
