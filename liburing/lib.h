/* SPDX-License-Identifier: MIT */
#ifndef LIBURING_HDR_ONLY_LIB_H
#define LIBURING_HDR_ONLY_LIB_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#if defined(__x86_64__) || defined(__i386__)
#    include "arch/x86/lib.h"
#elif defined(__aarch64__)
#    include "arch/aarch64/lib.h"
#elif defined(__riscv) && __riscv_xlen == 64
#    include "arch/riscv64/lib.h"
#else
/*
 * We don't have nolibc support for this arch. Must use libc!
 */
#    ifdef CONFIG_NOLIBC
#        error "This arch doesn't support building liburing without libc"
#    endif
/* libc wrappers. */
#    include "arch/generic/lib.h"
#endif


#ifndef offsetof
#    define offsetof(TYPE, FIELD) ((size_t) & ((TYPE*) 0)->FIELD)
#endif

#ifndef container_of
#    define container_of(PTR, TYPE, FIELD)                         \
        ({                                                         \
            __typeof__(((TYPE*) 0)->FIELD)* __FIELD_PTR = (PTR);   \
            (TYPE*) ((char*) __FIELD_PTR - offsetof(TYPE, FIELD)); \
        })
#endif

#ifdef __cplusplus
#    define uring__maybe_unused [[maybe_unused]]
#    define uring__hot
#    define uring__cold
#else
#    define uring__maybe_unused __attribute__((__unused__))
#    define uring__hot          __attribute__((uring__hot__))
#    define uring__cold         __attribute__((uring__cold__))
#endif

#ifdef CONFIG_NOLIBC
static inline void* internal__uring_memset(void* s, int c, size_t n) {
    size_t         i;
    unsigned char* p = s;

    for (i = 0; i < n; i++) {
        p[i] = (unsigned char) c;

        /*
         * An empty inline ASM to avoid auto-vectorization
         * because it's too bloated for liburing.
         */
        __asm__ volatile("");
    }

    return s;
}

struct uring_heap {
    size_t len;
    char   user_p[] __attribute__((__aligned__));
};

static inline void* internal__uring_malloc(size_t len) {
    struct uring_heap* heap;

    heap = internal__sys_mmap(nullptr,
                              sizeof(*heap) + len,
                              PROT_READ | PROT_WRITE,
                              MAP_ANONYMOUS | MAP_PRIVATE,
                              -1,
                              0);
    if (IS_ERR(heap))
        return nullptr;

    heap->len = sizeof(*heap) + len;
    return heap->user_p;
}

static inline void internal__uring_free(void* p) {
    struct uring_heap* heap;

    if (uring_unlikely(!p))
        return;

    heap = container_of(p, struct uring_heap, user_p);
    internal__sys_munmap(heap, heap->len);
}

#    define malloc(LEN)         internal__uring_malloc(LEN)
#    define free(PTR)           internal__uring_free(PTR)
#    define memset(PTR, C, LEN) internal__uring_memset(PTR, C, LEN)
#endif

#endif /* #ifndef LIBURING_HDR_ONLY_LIB_H */
