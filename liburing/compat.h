/* SPDX-License-Identifier: MIT */
#ifndef LIBURING_HDR_ONLY_COMPAT_H
#define LIBURING_HDR_ONLY_COMPAT_H

#include <linux/version.h>

/// __kernel_timespec

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
typedef int __kernel_rwf_t;
#endif



/// __kernel_timespec

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 18, 0)
#    include <stdint.h>
struct __kernel_timespec {
    int64_t   tv_sec;
    long long tv_nsec;
};
/* <linux/time_types.h> is not available, so it can't be included */
#    define UAPI_LINUX_IO_URING_H_SKIP_LINUX_TIME_TYPES_H 1
#else

#    include <linux/time_types.h>
/* <linux/time_types.h> is included above and not needed again */
#    define UAPI_LINUX_IO_URING_H_SKIP_LINUX_TIME_TYPES_H 1
#endif




/// open_how

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 6, 0)
#    include <inttypes.h>
struct open_how {
    uint64_t flags;
    uint64_t mode;
    uint64_t resolve;
};
#else

#    include <linux/openat2.h>

#endif


#ifdef __cplusplus
#    define uring_nodiscard                  [[nodiscard]]
#    define uring_static_cast(To, What)      static_cast<To>(What)
#    define uring_reinterpret_cast(To, What) reinterpret_cast<To>(What)
#else
#    define noexcept
#    define nullptr NULL
#    define uring_nodiscard
#    define uring_static_cast(To, What)      (To)(What)
#    define uring_reinterpret_cast(To, What) (To)(What)
#endif

#define UNUSED(x) (void) (x)
// todo: check for statx


/// This little utility is designed to get away with C and C++ not
/// allowing pointer arithmetic on void pointers.
#ifdef __cplusplus
// #    include <cstdint>
// #    include <iterator>
//// This is commented out because the C implementation's non-void-pointers implementation yields warning
//// when it's actually void(const)*.
// template <typename T,
//           typename NType = typename std::iterator_traits<T *>::difference_type>
// [[nodiscard]] static constexpr auto io_uring_next_void_ptr(T *ptr, NType n = 1) noexcept {
//     return std::next(
//       ptr, static_cast<typename std::iterator_traits<T *>::difference_type>(n));
// }
template <typename NType = std::ptrdiff_t>
[[nodiscard]] static inline auto io_uring_next_void_ptr(void const* ptr, NType n = 1) noexcept {
    return static_cast<void const*>(static_cast<char const*>(ptr) + n);
    // return std::next(reinterpret_cast<char const*>(ptr), static_cast<std::ptrdiff_t>(n));
}
template <typename NType = std::ptrdiff_t>
[[nodiscard]] static inline auto io_uring_next_void_ptr(void* ptr, NType n = 1) noexcept {
    // from: https://en.cppreference.com/w/cpp/language/lifetime
    //  Pointers to storage without an object that were cast to possibly cv-qualified void* can only be
    //  static_cast to pointers to possibly cv-qualified char, or possibly cv-qualified unsigned char, or
    //  possibly cv-qualified std::byte (since C++17).
    return static_cast<void*>(static_cast<char*>(ptr) + n);
    // return std::next(reinterpret_cast<char*>(ptr), static_cast<std::ptrdiff_t>(n));
}
#else
// #    include <stdint.h>
#    define io_uring_next_void_ptr(ptr, N)    \
        _Generic((ptr),                       \
          void*: ((void*) ((char*) ptr + N)), \
          void const*: ((void const*) ((char const*) ptr + N)))
#endif


#ifdef __cplusplus
#    include <memory>
#    ifdef __cpp_lib_start_lifetime_as
#        define io_uring_start_lifetime(type, ptr)             std::start_lifetime_as<type>(ptr)
#        define io_uring_start_lifetime_array(type, ptr, size) std::start_lifetime_as<type>(ptr, size)
#    else
#        define io_uring_start_lifetime(type, ptr)             reinterpret_cast<type*>(ptr)
#        define io_uring_start_lifetime_array(type, ptr, size) reinterpret_cast<type*>(ptr)
#    endif
#else
#    define io_uring_start_lifetime(type, ptr)       (type*) (ptr)
#    define io_uring_start_lifetime_array(type, ptr) (type*) (ptr)
#endif


#endif
