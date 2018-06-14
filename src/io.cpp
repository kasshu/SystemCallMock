#include "util/io.h"

//#ifdef UNITTEST
//#include "test/mock_system_func.h"
//#endif

template <class Func>
static ssize_t NonePosixIoWrapper(Func func, int fd, const struct iovec *vector,
                           int count, off_t offset) {
    ssize_t bytes_total = 0;
    for (int i = 0; i < count; ++i) {
        const ssize_t bytes = func(fd, vector[i].iov_base, vector[i].iov_len, offset);
        if (bytes <= 0) {
            return bytes_total > 0 ? bytes_total : bytes;
        }
        bytes_total += bytes;
        offset += bytes;
        if (bytes < (ssize_t)vector[i].iov_len) {
            break;
        }
    }
    return bytes_total;
}

#if defined(__APPLE__)
ssize_t preadv(int fd, const iovec* iov, int count, off_t offset) {
    return NonePosixIoWrapper(pread, fd, iov, count, offset);
}
ssize_t pwritev(int fd, const iovec* iov, int count, off_t offset) {
    return NonePosixIoWrapper(pwrite, fd, iov, count, offset);
}
#endif

namespace util {

ssize_t UninterruptedPread(int fd, void * buffer, size_t count, off_t offset) {
    size_t bytes_left = count;
    while(bytes_left > 0) {
        auto bytes_read = pread(fd, buffer, bytes_left, offset);
        if (bytes_read > 0) {
            bytes_left -= bytes_read;
            offset += bytes_read;
        } else if (bytes_read == 0) {
            break;
        } else if (errno == EINTR) {
            continue;
        } else {
            return bytes_read;
        }
    }

    return count - bytes_left;
}

ssize_t UninterruptedPwrite(int fd, void * buffer, size_t count, off_t offset) {
    size_t bytes_left = count;
    while(bytes_left > 0) {
        auto bytes_written = pwrite(fd, buffer, bytes_left, offset);
        if (bytes_written > 0) {
            bytes_left -= bytes_written;
            offset += bytes_written;
        } else if (bytes_written == 0) {
            break;
        } else if (errno == EINTR) {
            continue;
        } else {
            return bytes_written;
        }
    }

    return count - bytes_left;
}

ssize_t UninterruptedPwritev(int fd, iovec *iov, int iovcnt, off_t offset) {
    int cur = 0;
    off_t origin_offset = offset;
    while (true) {
        auto bytes_written = pwritev(fd, iov + cur, iovcnt - cur, offset);
        if(bytes_written > 0) {
            offset += bytes_written;
            //Find the first iovec should be written
            while (cur < iovcnt && bytes_written >= iov[cur].iov_len) {
                bytes_written -= iov[cur++].iov_len;
            }
            //Break the loop if all iovecs have been written
            if(cur == iovcnt) {
                break;
            }
            iov[cur].iov_base = static_cast<char *>(iov[cur].iov_base) + bytes_written;
            iov[cur].iov_len -= bytes_written;

        } else if (bytes_written == 0) {
            break;
        } else if (errno == EINTR) {
            continue;
        } else {
            return bytes_written;
        }
    }

    return offset - origin_offset;
}

ssize_t UninterruptedPreadv(int fd, iovec *iov, int iovcnt, off_t offset) {
    int cur = 0;
    off_t origin_offset = offset;
    while (true) {
        auto bytes_read = preadv(fd, iov + cur, iovcnt - cur, offset);
        if(bytes_read > 0) {
            offset += bytes_read;
            //Find the first iovec should be written
            while (cur < iovcnt && bytes_read >= iov[cur].iov_len) {
                bytes_read -= iov[cur++].iov_len;
            }
            //Break the loop if all iovecs have been written
            if(cur == iovcnt) {
                break;
            }
            iov[cur].iov_base = static_cast<char *>(iov[cur].iov_base) + bytes_read;
            iov[cur].iov_len -= bytes_read;

        } else if (bytes_read == 0) {
            break;
        } else if (errno == EINTR) {
            continue;
        } else {
            return bytes_read;
        }
    }

    return offset - origin_offset;
}

}//end of namespace util
