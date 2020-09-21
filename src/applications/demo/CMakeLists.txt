cmake_minimum_required(VERSION 2.8)

project(CacheDemo)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

file(GLOB CacheDemo_sources_tests ./*.cpp)
file(GLOB CacheDemo_sources_tests_h ./*.h)
file(GLOB CacheDemo_sources_tclap_h ${CMAKE_CURRENT_SOURCE_DIR}../../thirdParty/tclap/*.h)

list(APPEND CacheDemo_sources ${CacheDemo_sources_tests})
list(APPEND CacheDemo_sources ${CacheDemo_sources_tests_h})
list(APPEND CacheDemo_sources ${CacheDemo_sources_tclap_h})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../libs/cachelib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../thirdParty)

if(MSVC)
    foreach(f ${CacheDemo_sources})
        # Get the path of the file relative to ${CMAKE_HOME_DIRECTORY},
        # then alter it (not compulsory)
        file(RELATIVE_PATH SRCGR ${CMAKE_HOME_DIRECTORY} ${f})
        set(SRCGR "${SRCGR}")

        # Extract the folder, ie remove the filename part
        string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})

        # Source_group expects \\ (double antislash), not / (slash)
        string(REPLACE / \\ SRCGR ${SRCGR})
        source_group("${SRCGR}" FILES ${f})
    endforeach()
endif()

link_directories(${Cache_LINKER_LIBS})

add_executable(CacheDemo ${CacheDemo_sources})
target_compile_options(CacheDemo PRIVATE ${CACHE_CXX_FLAGS})

target_link_libraries(CacheDemo CacheLib)

install(TARGETS CacheDemo DESTINATION ${CMAKE_INSTALL_PREFIX})