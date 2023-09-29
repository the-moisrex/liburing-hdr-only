/* SPDX-License-Identifier: MIT */

#ifndef LIBURING_ARCH_AARCH64_SYSCALL_H
#define LIBURING_ARCH_AARCH64_SYSCALL_H

#if defined(__aarch64__)

#    define internal__do_syscallN(...)                                       \
        __asm__ volatile("svc 0" : "=r"(x0) : __VA_ARGS__ : "memory", "cc"); \
        const long ret = (long) x0

#    define internal__do_syscall0(__n)        \
        register long x8 __asm__("x8") = __n; \
        register long x0 __asm__("x0");       \
                                              \
        internal__do_syscallN("r"(x8))

#    define internal__do_syscall1(__n, __a)              \
        register long            x8 __asm__("x8") = __n; \
        register __typeof__(__a) x0 __asm__("x0") = __a; \
                                                         \
        internal__do_syscallN("r"(x8), "0"(x0))

#    define internal__do_syscall2(__n, __a, __b)         \
        register long            x8 __asm__("x8") = __n; \
        register __typeof__(__a) x0 __asm__("x0") = __a; \
        register __typeof__(__b) x1 __asm__("x1") = __b; \
                                                         \
        internal__do_syscallN("r"(x8), "0"(x0), "r"(x1))

#    define internal__do_syscall3(__n, __a, __b, __c)    \
        register long            x8 __asm__("x8") = __n; \
        register __typeof__(__a) x0 __asm__("x0") = __a; \
        register __typeof__(__b) x1 __asm__("x1") = __b; \
        register __typeof__(__c) x2 __asm__("x2") = __c; \
                                                         \
        internal__do_syscallN("r"(x8), "0"(x0), "r"(x1), "r"(x2))

#    define internal__do_syscall4(__n, __a, __b, __c, __d) \
        register long            x8 __asm__("x8") = __n;   \
        register __typeof__(__a) x0 __asm__("x0") = __a;   \
        register __typeof__(__b) x1 __asm__("x1") = __b;   \
        register __typeof__(__c) x2 __asm__("x2") = __c;   \
        register __typeof__(__d) x3 __asm__("x3") = __d;   \
                                                           \
        internal__do_syscallN("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3))

#    define internal__do_syscall5(__n, __a, __b, __c, __d, __e) \
        register long            x8 __asm__("x8") = __n;        \
        register __typeof__(__a) x0 __asm__("x0") = __a;        \
        register __typeof__(__b) x1 __asm__("x1") = __b;        \
        register __typeof__(__c) x2 __asm__("x2") = __c;        \
        register __typeof__(__d) x3 __asm__("x3") = __d;        \
        register __typeof__(__e) x4 __asm__("x4") = __e;        \
                                                                \
        internal__do_syscallN("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4))

#    define internal__do_syscall6(__n, __a, __b, __c, __d, __e, __f) \
        register long            x8 __asm__("x8") = __n;             \
        register __typeof__(__a) x0 __asm__("x0") = __a;             \
        register __typeof__(__b) x1 __asm__("x1") = __b;             \
        register __typeof__(__c) x2 __asm__("x2") = __c;             \
        register __typeof__(__d) x3 __asm__("x3") = __d;             \
        register __typeof__(__e) x4 __asm__("x4") = __e;             \
        register __typeof__(__f) x5 __asm__("x5") = __f;             \
                                                                     \
        internal__do_syscallN("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5))

#    include "../syscall-defs.h"

#else /* #if defined(__aarch64__) */

#    include "../generic/syscall.h"

#endif /* #if defined(__aarch64__) */

#endif /* #ifndef LIBURING_ARCH_AARCH64_SYSCALL_H */
