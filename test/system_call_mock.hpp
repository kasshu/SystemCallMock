#include <gmock/gmock.h>
#include <functional>

static std::function<ssize_t(int, void*, size_t, off_t)> _pread;
static std::function<ssize_t(int, const void*, size_t, off_t)> _pwrite;
static std::function<ssize_t(int, const struct iovec*, int, off_t)> _preadv;
static std::function<ssize_t(int, const struct iovec*, int, off_t)> _pwritev;
static std::function<int(int)> _fsync;
static std::function<int(int)> _close;
static std::function<int(const char*)> _unlink;
static std::function<int(const char*, int)> _open;
static std::function<int(int, off_t)> _ftruncate;
static std::function<int(int, struct stat*)> _fstat;

class SystemCallMocker {
public:
    SystemCallMocker() {
        _pread = [this](int fd, void *buffer, size_t count, off_t offset){
            return pread(fd, buffer, count, offset);
        };

        _pwrite = [this](int fd, const void *buffer, size_t count, off_t offset){
            return pwrite(fd, buffer, count, offset);
        };

        _preadv = [this](int fd, const struct iovec *iov, int iovcnt, off_t offset){
            return preadv(fd, iov, iovcnt, offset);
        };

        _pwritev = [this](int fd, const iovec *iov, int iovcnt, off_t offset){
            return pwritev(fd, iov, iovcnt, offset);
        };

        _fsync = [this](int fd){
            return fsync(fd);
        };

        _close = [this](int fd){
            return close(fd);
        };

        _unlink = [this](const char *pathname){
            return unlink(pathname);
        };

        _open = [this](const char *pathname, int flags){
            return open(pathname, flags);
        };

        _ftruncate = [this](int fd, off_t length){
            return ftruncate(fd, length);
        };

        _fstat = [this](int fd, struct stat *buf){
            return fstat(fd, buf);
        };
    }

    ~SystemCallMocker() {
        _pread = {};
        _pwrite = {};
        _preadv = {};
        _pwritev = {};
        _fsync = {};
        _close = {};
        _unlink = {};
        _open = {};
        _ftruncate = {};
        _fstat = {};
    }

    MOCK_CONST_METHOD4(pread, ssize_t(int, void*, size_t, off_t));
    MOCK_CONST_METHOD4(pwrite, ssize_t(int, const void*, size_t, off_t));
    MOCK_CONST_METHOD4(preadv, ssize_t(int, const struct iovec*, int, off_t));
    MOCK_CONST_METHOD4(pwritev, ssize_t(int, const iovec*, int, off_t));
    MOCK_CONST_METHOD1(fsync, int(int));
    MOCK_CONST_METHOD1(close, int(int));
    MOCK_CONST_METHOD1(unlink, int(const char*));
    MOCK_CONST_METHOD2(open, int(const char*, int));
    MOCK_CONST_METHOD2(ftruncate, int(int, off_t));
    MOCK_CONST_METHOD2(fstat, int(int, struct stat*));
};

class SystemCallMockBase {
protected:
    SystemCallMocker scm;
};

#ifdef __cplusplus
extern "C" {
#endif

ssize_t pread(int fd, void *buffer, size_t count, off_t offset) {
    return _pread(fd, buffer, count, offset);
}

ssize_t pwrite(int fd, const void *buffer, size_t count, off_t offset) {
    return _pwrite(fd, buffer, count, offset);
}

ssize_t preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset) {
    return _preadv(fd, iov, iovcnt, offset);
}

ssize_t pwritev(int fd, const iovec *iov, int iovcnt, off_t offset) {
    return _pwritev(fd, iov, iovcnt, offset);
}

int fsync(int fd) {
    return _fsync(fd);
}

int close(int fd) {
    return _close(fd);
}

int unlink(const char *pathname) {
    return _unlink(pathname);
}

int open(const char *pathname, int flags) {
    return _open(pathname, flags);
}

int ftruncate(int fd, off_t length) {
    return _ftruncate(fd, length);
}

int fstat(int fd, struct stat *buf) {
    return _fstat(fd, buf);
}

#ifdef __cplusplus
}
#endif
