cmake_minimum_required(VERSION 3.10)

project(unqlite_cpp_tests VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/3rdparty/doctest)

add_executable(unqlite_cpp_tests
    ${CMAKE_CURRENT_LIST_DIR}/unqlite_cpp_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/unqlite/unqlite.c
    ${CMAKE_CURRENT_LIST_DIR}/value_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/vm_value_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/db_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/kv_cursor_tests.cpp
    )

target_include_directories(unqlite_cpp_tests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../include)
target_include_directories(unqlite_cpp_tests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/unqlite)

target_link_libraries(unqlite_cpp_tests PUBLIC doctest::doctest)

if(${UNQLITEPP_ALLOW_EXCEPTIONS})
    add_definitions(-DUNQLITEPP_ALLOW_EXCEPTIONS)
endif()
