cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Cache)

option(CACHE_BUILD_TESTS "Configure Tests target" ON)
option(CACHE_BUILD_DEMO "Configure Demo target" ON)

# CACHE project directory
set(CACHE_ROOT ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(src/libs/cachelib)

if(CACHE_BUILD_TESTS)
  add_subdirectory(src/applications/tests)
endif()

if(CACHE_BUILD_DEMO)
  add_subdirectory(src/applications/demo)
endif()