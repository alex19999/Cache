cmake_minimum_required(VERSION 3.10)

project(CacheLib)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE Cache_sources_main_cpp cache/*.cpp)
file(GLOB_RECURSE Cache_sources_main_h cache/*.h)

list(APPEND Cache_sources ${Cache_sources_main_cpp})
list(APPEND Cache_sources ${Cache_sources_main_h})

include_directories(.)

add_definitions(-DAPI_EXPORTS)

if(MSVC)
  foreach(f ${Cache_sources})
    file(RELATIVE_PATH SRCGR ${CMAKE_HOME_DIRECTORY} ${f})
    set(SRCGR "${SRCGR}")

    string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})

    string(REPLACE / \\ SRCGR ${SRCGR})
    source_group("${SRCGR}" FILES ${f})
  endforeach()
endif()

link_directories(${Cache_LINKER_LIBS})

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

add_library(CacheLib SHARED ${Cache_sources})
target_compile_options(CacheLib PRIVATE ${CACHE_CXX_FLAGS} ${CACHE_LINK_FLAGS})

if(NOT WIN32)
  target_link_libraries(CacheLib stdc++fs)
endif()

target_include_directories(CacheLib ${Cache_INCLUDE_DIRS} PUBLIC)

if(WIN32)
  install(
    TARGETS CacheLib
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
else()
  install(TARGETS CacheLib DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()