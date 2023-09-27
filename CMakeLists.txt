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
else ()
    set_target_properties(${PROJECT_NAME} PROPERTIES
            LINKER_LANGUAGE C
    )
endif ()

target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET HEADERS
        FILES
            include/liburing.h
            include/liburing-hdr-only.h
            include/lib.h
            include/io_uring_version.h
            include/io_uring.h
            include/compat.h
            include/barrier.h
            include/arch/syscall-defs.h
            include/arch/aarch64/lib.h
            include/arch/aarch64/syscall.h
            include/arch/x86/lib.h
            include/arch/x86/syscall.h
            include/arch/generic/lib.h
            include/arch/generic/syscall.h
            include/arch/riscv64/lib.h
            include/arch/riscv64/syscall.h
)

target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:include>)

install(TARGETS ${PROJECT_NAME}
        EXPORT  ${PROJECT_NAME}Config
        FILE_SET HEADERS
        DESTINATION include/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}Config
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})
