#ifndef UTIL_IO_H
#define UTIL_IO_H

#include <errno.h>
#include <unistd.h>
#include <sys/uio.h>

namespace util {

//Pread/preadv/pwrite/pwritev is thread safe
ssize_t UninterruptedPread(int fd, void * buffer, size_t count, off_t offset);

ssize_t UninterruptedPwrite(int fd, void * buffer, size_t count, off_t offset);

ssize_t UninterruptedPreadv(int fd, iovec *iov, int iovcnt, off_t offset);

ssize_t UninterruptedPwritev(int fd, iovec *iov, int iovcnt, off_t offset);

}//end of namespace util

#endif //UTIL_IO_H
