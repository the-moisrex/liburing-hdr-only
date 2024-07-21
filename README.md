# Liburing header-only

This is an implementation of liburing that's header only and trying to be standard compliant (which liburing itself is not).

### Goals of this fork

- **Header Only**
- **No Undefined Behaviours**
- **Standard compliant (No compiler-specific features)**
- **-pedantic friendly**
- **C and C++ compatibility** without any `extern "C"` or `extern "C++"`es

This library is a fork of liburing 2.4 (or 2.5, [here's the exact commit](https://github.com/axboe/liburing/commit/b58921e0b0ae84b6f1cd22b87c66a6e91a540ec8)); 
we've kept up with the changes so far (version 2.7).

### C++ users

- The original liburing library, is not very C++ friendly
- Use this fork even if you prefer a non-header-only version for C++
- The primary aim of this fork is for C++ users (at least currently that is)
- This fork trys to stay compliant with the upstream's API
- All the functions are marked as `noexcept` for C++
- There a few compiler-specific features and UBs remaining that will be fixed

### Usage in CMake

Using [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) (Use a release link instead of the master branch, and adjust the version):
```cmake
CPMAddPackage(
        NAME liburing
        URL https://github.com/the-moisrex/liburing-hdr-only/archive/refs/heads/master.zip
        VERSION 2.7
        OPTIONS
            "LIBURING_CXX ON"
)
```

### Using original liburing, and fallback on the header only version

```c++
#ifdef __linux__
#    if defined(USE_OS_LIBURING) && __has_include(<liburing.h>)
#        include <liburing.h>
#    else
#        include <liburing/liburing-hdr-only.h>
#    endif
#endif
```

Then define `USE_OS_LIBURING` in your build system (`-DUSE_OS_LIBURING`) to enable the use of original liburing if it exists,
and fallback on the header-only version otherwise.


### Using header-only version, and fallback on the original version if it doesn't exist

```c++
#ifdef __linux__
#    if __has_include(<liburing/liburing-hdr-only.h>)
#        include <liburing/liburing-hdr-only.h>
#    else
#        include <liburing.h>
#    endif
#endif
```
