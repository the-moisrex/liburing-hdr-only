/* SPDX-License-Identifier: MIT */

#ifndef LIBURING_ARCH_RISCV64_SYSCALL_H
#define LIBURING_ARCH_RISCV64_SYSCALL_H

#if defined(__riscv) && __riscv_xlen == 64

#    define internal__do_syscallM(...)                                       \
        __asm__ volatile("ecall" : "=r"(a0) : __VA_ARGS__ : "memory", "a1"); \
        const long ret = (long) a0

#    define internal__do_syscallN(...)                                 \
        __asm__ volatile("ecall" : "=r"(a0) : __VA_ARGS__ : "memory"); \
        const long ret = (long) a0

#    define internal__do_syscall0(__n)        \
        register long a7 __asm__("a7") = __n; \
        register long a0 __asm__("a0");       \
                                              \
        internal__do_syscallM("r"(a7))

#    define internal__do_syscall1(__n, __a)              \
        register long            a7 __asm__("a7") = __n; \
        register __typeof__(__a) a0 __asm__("a0") = __a; \
                                                         \
        internal__do_syscallM("r"(a7), "0"(a0))

#    define internal__do_syscall2(__n, __a, __b)         \
        register long            a7 __asm__("a7") = __n; \
        register __typeof__(__a) a0 __asm__("a0") = __a; \
        register __typeof__(__b) a1 __asm__("a1") = __b; \
                                                         \
        internal__do_syscallN("r"(a7), "0"(a0), "r"(a1))

#    define internal__do_syscall3(__n, __a, __b, __c)    \
        register long            a7 __asm__("a7") = __n; \
        register __typeof__(__a) a0 __asm__("a0") = __a; \
        register __typeof__(__b) a1 __asm__("a1") = __b; \
        register __typeof__(__c) a2 __asm__("a2") = __c; \
                                                         \
        internal__do_syscallN("r"(a7), "0"(a0), "r"(a1), "r"(a2))

#    define internal__do_syscall4(__n, __a, __b, __c, __d) \
        register long            a7 __asm__("a7") = __n;   \
        register __typeof__(__a) a0 __asm__("a0") = __a;   \
        register __typeof__(__b) a1 __asm__("a1") = __b;   \
        register __typeof__(__c) a2 __asm__("a2") = __c;   \
        register __typeof__(__d) a3 __asm__("a3") = __d;   \
                                                           \
        internal__do_syscallN("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))

#    define internal__do_syscall5(__n, __a, __b, __c, __d, __e) \
        register long            a7 __asm__("a7") = __n;        \
        register __typeof__(__a) a0 __asm__("a0") = __a;        \
        register __typeof__(__b) a1 __asm__("a1") = __b;        \
        register __typeof__(__c) a2 __asm__("a2") = __c;        \
        register __typeof__(__d) a3 __asm__("a3") = __d;        \
        register __typeof__(__e) a4 __asm__("a4") = __e;        \
                                                                \
        internal__do_syscallN("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))

#    define internal__do_syscall6(__n, __a, __b, __c, __d, __e, __f) \
        register long            a7 __asm__("a7") = __n;             \
        register __typeof__(__a) a0 __asm__("a0") = __a;             \
        register __typeof__(__b) a1 __asm__("a1") = __b;             \
        register __typeof__(__c) a2 __asm__("a2") = __c;             \
        register __typeof__(__d) a3 __asm__("a3") = __d;             \
        register __typeof__(__e) a4 __asm__("a4") = __e;             \
        register __typeof__(__f) a5 __asm__("a5") = __f;             \
                                                                     \
        internal__do_syscallN("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))

#    include "../syscall-defs.h"

#else /* #if defined(__riscv) && __riscv_xlen == 64 */

#    include "../generic/syscall.h"

#endif /*  #if defined(__riscv) && __riscv_xlen == 64 */

#endif /* #ifndef LIBURING_ARCH_RISCV64_SYSCALL_H */
