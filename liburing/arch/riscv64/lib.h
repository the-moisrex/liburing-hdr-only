/* SPDX-License-Identifier: MIT */

#ifndef LIBURING_ARCH_RISCV64_LIB_H
#define LIBURING_ARCH_RISCV64_LIB_H

#include "./syscall.h"

#include <elf.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <unistd.h>


static inline long __get_page_size(void) {
    Elf64_Off buf[2];
    long      ret = 4096;
    int       fd;

    fd = internal__sys_open("/proc/self/auxv", O_RDONLY, 0);
    if (fd < 0)
        return ret;

    while (1) {
        ssize_t x;

        x = internal__sys_read(fd, buf, sizeof(buf));
        if (x < (long) sizeof(buf))
            break;

        if (buf[0] == AT_PAGESZ) {
            ret = buf[1];
            break;
        }
    }

    internal__sys_close(fd);
    return ret;
}

static inline long get_page_size(void) {
    static long cache_val;

    if (cache_val)
        return cache_val;

    cache_val = __get_page_size();
    return cache_val;
}

#endif /* #ifndef LIBURING_ARCH_RISCV64_LIB_H */
