cmake_minimum_required(VERSION 3.20)

project(WebSocketServer CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TARGET_NAME websocket_server)
include(FetchContent)

FetchContent_Declare(
  tec.h
  GIT_REPOSITORY https://github.com/ShashwatAgrawal20/tec.h.git
  GIT_TAG        v1.1.0
)
FetchContent_MakeAvailable(tec.h)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

set(USOCKETS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/uwebsockets/uSockets)
set(USOCKETS_LIBRARY ${USOCKETS_SOURCE_DIR}/uSockets.a)

add_custom_command(
    OUTPUT ${USOCKETS_LIBRARY}
    COMMAND ${CMAKE_MAKE_PROGRAM} WITH_OPENSSL=1 -C ${USOCKETS_SOURCE_DIR}
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
add_dependencies(uSockets build_usockets)

add_library(uWebSockets_Interface INTERFACE)
target_include_directories(uWebSockets_Interface INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(uWebSockets_Interface INTERFACE uSockets ZLIB::ZLIB OpenSSL::SSL OpenSSL::Crypto)

file(GLOB_RECURSE TARGET_SOURCES "src/*.cpp")
list(FILTER TARGET_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

add_library(app_lib STATIC ${TARGET_SOURCES})
target_link_libraries(app_lib PUBLIC
    uWebSockets_Interface
    nlohmann_json::nlohmann_json
)
target_include_directories(app_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
add_dependencies(app_lib build_usockets)

add_executable(${TARGET_NAME} src/main.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE app_lib)

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
if(BUILD_TESTING)
    file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
    add_executable(tests EXCLUDE_FROM_ALL
        ${TEST_SOURCES}
    )
    target_link_libraries(tests PRIVATE app_lib)
    target_include_directories(tests PUBLIC
        ${tec.h_SOURCE_DIR}
    )
    if(MSVC)
      target_compile_options(tests PRIVATE /W4 /WX)
    else()
      target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    add_test(NAME tests COMMAND tests)
    add_custom_target(check
        COMMAND ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1 ./tests || true
        DEPENDS tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()
