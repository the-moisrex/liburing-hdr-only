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



#ifdef __cplusplus
#    include <iterator>
//// This is commented out because the C implementation's non-void-pointers implementation yields warning
//// when it's actually void(const)*.
// template <typename T,
//           typename NType = typename std::iterator_traits<T *>::difference_type>
// [[nodiscard]] static constexpr auto next_ptr(T *ptr, NType n = 1) noexcept {
//     return std::next(
//       ptr, static_cast<typename std::iterator_traits<T *>::difference_type>(n));
// }
template <typename NType = std::ptrdiff_t>
[[nodiscard]] static inline auto next_ptr(void const* ptr, NType n = 1) noexcept {
    return std::next(reinterpret_cast<std::intptr_t const*>(ptr), static_cast<std::ptrdiff_t>(n));
}
template <typename NType = std::ptrdiff_t>
[[nodiscard]] static inline auto next_ptr(void* ptr, NType n = 1) noexcept {
    return std::next(reinterpret_cast<std::intptr_t*>(ptr), static_cast<std::ptrdiff_t>(n));
}
#else
#    include <stdint.h>
#    define next_ptr(ptr, N)                      \
        _Generic((ptr),                           \
          void*: ((void*) ((intptr_t*) ptr + N)), \
          void const*: ((void const*) ((intptr_t const*) ptr + N)))
#endif

#endif
