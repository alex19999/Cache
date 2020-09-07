cmake_minimum_required(VERSION 3.10)

project(CacheTests)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
file(RELATIVE_PATH REL_CACHE_ROOT ${CMAKE_INSTALL_PREFIX} ${CMAKE_CURRENT_SOURCE_DIR}/../../../)

if(REL_CACHE_ROOT STREQUAL "")
    set(REL_CACHE_ROOT ".")
endif()

file(GLOB_RECURSE CacheTests_sources_tests ./*.cpp)
file(GLOB_RECURSE CacheTests_sources_tests_h ./*.h)

# Google Test Initialization
file(GLOB CacheTests_gtest ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdParty/gtest/src/gtest-all.cc)
file(GLOB CacheTests_gtest_h
     ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdParty/gtest/include/gtest/*.h)

list(APPEND CacheTests_sources ${CacheTests_sources_tests})
list(APPEND CacheTests_sources ${CacheTests_sources_tests_h})
list(APPEND CacheTests_sources ${CacheTests_gtest})
list(APPEND CacheTests_sources ${CacheTests_gtest_h})

add_definitions(-DGTEST_HAS_PTHREAD=0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../thirdParty/gtest)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../thirdParty/gtest/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../libs/cachelib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../applications/tests)

include(GoogleTest)
enable_testing()

if(MSVC)
  foreach(f ${CacheTests_sources})
    file(RELATIVE_PATH SRCGR ${CMAKE_HOME_DIRECTORY} ${f})
    set(SRCGR "${SRCGR}")

    string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})

    string(REPLACE / \\ SRCGR ${SRCGR})
    source_group("${SRCGR}" FILES ${f})
  endforeach()
endif()

link_directories(${Cache_LINKER_LIBS})

add_executable(CacheTests ${CacheTests_sources})
target_compile_options(CacheTests PRIVATE ${CACHE_CXX_FLAGS} ${CACHE_LINK_FLAGS})
target_link_libraries(CacheTests CacheLib)
if(NOT WIN32)
  target_link_libraries(CacheTests stdc++fs)
endif()

install(TARGETS CacheTests DESTINATION ${CMAKE_INSTALL_PREFIX})