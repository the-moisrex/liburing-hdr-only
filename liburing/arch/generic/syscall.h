/* SPDX-License-Identifier: MIT */

#ifndef LIBURING_ARCH_GENERIC_SYSCALL_H
#define LIBURING_ARCH_GENERIC_SYSCALL_H

#include <errno.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <unistd.h>

static inline int
internal__sys_io_uring_register(unsigned int fd, unsigned int opcode, const void* arg, unsigned int nr_args) {
    int const ret = (int) syscall(__NR_io_uring_register, fd, opcode, arg, nr_args);
    return (ret < 0) ? -errno : ret;
}

static inline int internal__sys_io_uring_setup(unsigned int entries, struct io_uring_params* p) {
    int const ret = (int) syscall(__NR_io_uring_setup, entries, p);
    return (ret < 0) ? -errno : ret;
}

static inline int internal__sys_io_uring_enter2(unsigned int fd,
                                                unsigned int to_submit,
                                                unsigned int min_complete,
                                                unsigned int flags,
                                                sigset_t*    sig,
                                                size_t       sz) {
    int const ret = (int) syscall(__NR_io_uring_enter, fd, to_submit, min_complete, flags, sig, sz);
    return (ret < 0) ? -errno : ret;
}

static inline int internal__sys_io_uring_enter(unsigned int fd,
                                               unsigned int to_submit,
                                               unsigned int min_complete,
                                               unsigned int flags,
                                               sigset_t*    sig) {
    return internal__sys_io_uring_enter2(fd, to_submit, min_complete, flags, sig, _NSIG / 8);
}

static inline int internal__sys_open(const char* pathname, int flags, mode_t mode) {
    int const ret = open(pathname, flags, mode);
    return (ret < 0) ? -errno : ret;
}

static inline ssize_t internal__sys_read(int fd, void* buffer, size_t size) {
    ssize_t const ret = read(fd, buffer, size);
    return (ret < 0) ? -errno : ret;
}

static inline void* internal__sys_mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset) {
    void* ret = mmap(addr, length, prot, flags, fd, offset);
    return (ret == MAP_FAILED) ? ERR_PTR(-errno) : ret;
}

static inline int internal__sys_munmap(void* addr, size_t length) {
    int const ret = munmap(addr, length);
    return (ret < 0) ? -errno : ret;
}

static inline int internal__sys_madvise(void* addr, size_t length, int advice) {
    int const ret = madvise(addr, length, advice);
    return (ret < 0) ? -errno : ret;
}

static inline int internal__sys_getrlimit(int resource, struct rlimit* rlim) {
    int const ret = getrlimit(resource, rlim);
    return (ret < 0) ? -errno : ret;
}

static inline int internal__sys_setrlimit(int resource, const struct rlimit* rlim) {
    int const ret = setrlimit(resource, rlim);
    return (ret < 0) ? -errno : ret;
}

static inline int internal__sys_close(int fd) {
    int const ret = close(fd);
    return (ret < 0) ? -errno : ret;
}

#endif /* #ifndef LIBURING_ARCH_GENERIC_SYSCALL_H */
