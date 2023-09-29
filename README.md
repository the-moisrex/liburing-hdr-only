# Liburing header-only

This is an implementation of liburing that's header only.

This is a work in progress right now.

### Goals of this fork

- **Header Only**
- **No Undefined Behaviours**
- **Standard compliant (No compiler-specific features)**
- **-pedantic friendly**
- **C and C++ compatibility** without any `extern "C"` or `extern "C++"`es

This library is a fork of liburing 2.4 (or 2.5, [here's the exact commit](https://github.com/axboe/liburing/commit/b58921e0b0ae84b6f1cd22b87c66a6e91a540ec8)).
