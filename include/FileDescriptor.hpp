/**
 * @file  FileDescriptor.h
 * @brief FileDescriptor
 *
 * Class definition for FileDescriptor
 *
 * @author     Clay Freeman
 * @date       March 13, 2015
 */

#ifndef _FILEDESCRIPTOR_H
#define _FILEDESCRIPTOR_H

class FileDescriptor {
  private:
    int fd = -1;
    // Make sure copying is disallowed
    FileDescriptor(const FileDescriptor&);
    FileDescriptor& operator= (const FileDescriptor&);
  public:
    FileDescriptor() = default;
    FileDescriptor(int fdi);
    ~FileDescriptor();
    int get() const;
    bool isValid() const;
    void reset();
    FileDescriptor& operator=(int fdi);
    operator int() const;
};

#endif
