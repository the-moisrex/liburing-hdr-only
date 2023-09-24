/* SPDX-License-Identifier: MIT */
#ifndef LIBURING_SYSCALL_H
#define LIBURING_SYSCALL_H

#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include "./liburing.h"

/*
 * Don't put this below the #include "arch/$arch/syscall.h", that
 * file may need it.
 */
struct io_uring_params;

static inline void *ERR_PTR(intptr_t n) {
    return (void *) n;
}

static inline int PTR_ERR(const void *ptr) {
    return (int) (intptr_t) ptr;
}

static inline bool IS_ERR(const void *ptr) {
    return uring_unlikely((uintptr_t) ptr >= (uintptr_t) -4095UL);
}

#if defined(__x86_64__) || defined(__i386__)

#include "arch/x86/syscall.h"

#elif defined(__aarch64__)
#include "arch/aarch64/syscall.h"
#elif defined(__riscv) && __riscv_xlen == 64
#include "arch/riscv64/syscall.h"
#else
/*
 * We don't have native syscall wrappers
 * for this arch. Must use libc!
 */
#ifdef CONFIG_NOLIBC
#error "This arch doesn't support building liburing without libc"
#endif
/* libc syscall wrappers. */
#include "arch/generic/syscall.h"
#endif


static inline int io_uring_enter(unsigned int fd, unsigned int to_submit,
                                 unsigned int min_complete, unsigned int flags, sigset_t *sig) noexcept {
    return __sys_io_uring_enter(fd, to_submit, min_complete, flags, sig);
}

static inline int io_uring_enter2(unsigned int fd, unsigned int to_submit,
                                  unsigned int min_complete, unsigned int flags,
                                  sigset_t *sig, size_t sz) noexcept {
    return __sys_io_uring_enter2(fd, to_submit, min_complete, flags, sig,
                                 sz);
}

static inline int io_uring_setup(unsigned int entries, struct io_uring_params *p) noexcept {
    return __sys_io_uring_setup(entries, p);
}

static inline int io_uring_register(unsigned int fd, unsigned int opcode, const void *arg,
                                    unsigned int nr_args) noexcept {
    return __sys_io_uring_register(fd, opcode, arg, nr_args);
}

#endif
