cmake_minimum_required(VERSION 3.24)

project(liburing
        VERSION 2.4.1
        LANGUAGES C CXX)

option(LIBURING_CXX "Use C++ instead of C" OFF)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::uring ALIAS ${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
)

if (LIBURING_CXX)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            LINKER_LANGUAGE CXX
    )
    # Cannot specify compile features for public targets:
    # target_compile_features(${name} PUBLIC
    #         cxx_std_20
    #         cxx_auto_type
    #         cxx_constexpr
    #         cxx_nullptr
    #         cxx_attributes
    #         cxx_noexcept
    #         cxx_uniform_initialization
    # )
else ()
    set_target_properties(${PROJECT_NAME} PROPERTIES
            LINKER_LANGUAGE C
    )
endif ()

target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET HEADERS
        FILES
        liburing/liburing.h
        liburing/liburing-hdr-only.h
        liburing/lib.h
        liburing/io_uring_version.h
        liburing/io_uring.h
        liburing/compat.h
        liburing/barrier.h
        liburing/arch/syscall-defs.h
        liburing/arch/aarch64/lib.h
        liburing/arch/aarch64/syscall.h
        liburing/arch/x86/lib.h
        liburing/arch/x86/syscall.h
        liburing/arch/generic/lib.h
        liburing/arch/generic/syscall.h
        liburing/arch/riscv64/lib.h
        liburing/arch/riscv64/syscall.h
)

target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
           $<INSTALL_INTERFACE:include>)

install(TARGETS ${PROJECT_NAME}
        EXPORT  ${PROJECT_NAME}Config
        FILE_SET HEADERS
        INCLUDES DESTINATION ${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}Config
        FILE ${PROJECT_NAME}Config.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/cmake/${PROJECT_NAME})

export(TARGETS ${PROJECT_NAME}
        FILE ${PROJECT_NAME}Config.cmake
)
