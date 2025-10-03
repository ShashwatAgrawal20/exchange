cmake_minimum_required(VERSION 3.20)

project(WebSocketServer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TARGET_NAME websocket_server)

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

set(USOCKETS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/uwebsockets/uSockets)
set(USOCKETS_LIBRARY ${USOCKETS_SOURCE_DIR}/uSockets.a)

add_custom_command(
    OUTPUT ${USOCKETS_LIBRARY}
    COMMAND make
    WORKING_DIRECTORY ${USOCKETS_SOURCE_DIR}
    COMMENT "Building uSockets library with its Makefile"
)

add_custom_target(
    build_usockets ALL
    DEPENDS ${USOCKETS_LIBRARY}
)

add_library(uSockets STATIC IMPORTED)
set_target_properties(uSockets PROPERTIES
    IMPORTED_LOCATION "${USOCKETS_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${USOCKETS_SOURCE_DIR}/src"
)

add_library(uWebSockets_Interface INTERFACE)

target_include_directories(uWebSockets_Interface INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(uWebSockets_Interface INTERFACE uSockets ZLIB::ZLIB OpenSSL::SSL OpenSSL::Crypto)

add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} PRIVATE uWebSockets_Interface)

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_dependencies(websocket_server build_usockets)
