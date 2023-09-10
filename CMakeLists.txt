cmake_minimum_required(VERSION 3.10)

project(
  unqlite_cpp
  VERSION 0.1.0
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(UNQLITEPP_TESTS CACHE BOOL FALSE)
set(UNQLITEPP_ALLOW_EXCEPTIONS CACHE BOOL FALSE)

add_library(unqlite_cpp INTERFACE)
add_library(unqlite_cpp::unqlite_cpp ALIAS unqlite_cpp)

target_include_directories(unqlite_cpp
                           INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

if(${UNQLITEPP_TESTS})
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
endif()

if(${UNQLITEPP_ALLOW_EXCEPTIONS})
  add_definitions(-DUNQLITEPP_ALLOW_EXCEPTIONS)
endif()
