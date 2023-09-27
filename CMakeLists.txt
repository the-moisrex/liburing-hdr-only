cmake_minimum_required(VERSION 3.24)

project(liburing
        VERSION 2.4.1
        LANGUAGES C CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::uring ALIAS ${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS ${PROJECT_NAME}
        EXPORT  ${PROJECT_NAME}Config
        FILE_SET HEADERS
        DESTINATION include/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}Config
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})
