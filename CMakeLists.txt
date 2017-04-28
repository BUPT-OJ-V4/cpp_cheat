cmake_minimum_required(VERSION 3.6)

project(cheat)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.64.0 REQUIRED COMPONENTS filesystem system thread thread-mt)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

add_definitions(-O2 --std=c++11 -Wall -Wextra -Wno-deprecated-declarations -DNDEBUG)
#add_definitions(--std=c++11)


set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBRARIES OFF)
# set(CMAKE_EXE_LINKER_FLAGS "-static")


# output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

link_directories("/usr/local/lib")
#link_directories("/home/work/3rdparty/gflags/lib")
#link_directories("/home/work/3rdparty/gtest/lib")
include_directories("/usr/local/include")

include_directories(SYSTEM "${Boost_INCLUDE_DIRS}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")
link_libraries(pthread)
set(CHEAT_SOURCES
        src/CheatWorker.cpp
        )

add_library(cheatlib STATIC ${CHEAT_SOURCES})

add_executable(CheatServer src/cheat_server.cpp)

target_link_libraries(CheatServer pthread)
target_link_libraries(CheatServer ${Boost_LIBRARIES})
target_link_libraries(CheatServer cheatlib)
