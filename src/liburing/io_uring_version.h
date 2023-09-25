/* SPDX-License-Identifier: MIT */
#ifndef LIBURING_VERSION_H
#define LIBURING_VERSION_H

#define IO_URING_VERSION_MAJOR 2
#define IO_URING_VERSION_MINOR 5


/*
 * Versioning information for liburing.
 *
 * Use IO_URING_CHECK_VERSION() for compile time checks including from
 * preprocessor directives.
 *
 * Use io_uring_check_version() for runtime checks of the version of
 * liburing that was loaded by the dynamic linker.
 */
int io_uring_major_version(void) {
    return IO_URING_VERSION_MAJOR;
}

int io_uring_minor_version(void) {
    return IO_URING_VERSION_MINOR;
}

bool io_uring_check_version(int major, int minor) {
    return major > io_uring_major_version() ||
           (major == io_uring_major_version() &&
            minor >= io_uring_minor_version());
}

#define IO_URING_CHECK_VERSION(major, minor) \
  (major > IO_URING_VERSION_MAJOR ||        \
   (major == IO_URING_VERSION_MAJOR &&      \
    minor >= IO_URING_VERSION_MINOR))


#endif
