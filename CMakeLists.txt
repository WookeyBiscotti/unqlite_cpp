cmake_minimum_required(VERSION 3.10)

project(
  unqlite_cpp
  VERSION 0.1.0
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(UNQLITE_CPP_TESTS CACHE BOOL FALSE)
set(UNQLITE_CPP_ALLOW_EXCEPTIONS CACHE BOOL FALSE)

add_library(unqlite_cpp INTERFACE)
add_library(unqlite_cpp::unqlite_cpp ALIAS unqlite_cpp)

target_include_directories(unqlite_cpp
                           INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

if(${UNQLITE_CPP_TESTS})
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
endif()

if(${UNQLITE_CPP_ALLOW_EXCEPTIONS})
  add_definitions(-DUNQLITE_CPP_ALLOW_EXCEPTIONS)
endif()
