cmake_minimum_required(VERSION 3.6)

project(cheat)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.58.0 REQUIRED COMPONENTS filesystem system thread)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

add_definitions(-D__KERNEL__=1)
add_definitions(-O2 --std=c++11 -Wall -Wextra -Wno-deprecated-declarations -DNDEBUG)
add_definitions(-DMYSQLCLIENT_STATIC_LINKING=1)
#add_definitions(--std=c++11)


set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBRARIES OFF)
# set(CMAKE_EXE_LINKER_FLAGS "-static")


# output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

link_directories("/usr/local/lib")
include_directories("/usr/local/include")
# include_directories("/usr/local/mysql/connector-c++-2.0/include")

include_directories(SYSTEM "${Boost_INCLUDE_DIRS}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")
link_libraries(pthread)
link_libraries(cpp_redis)
link_libraries(tacopie)
link_libraries(mysqlcppconn)

set(CHEAT_SOURCES
        src/CheatWorker.cpp
        src/cheat_algorithm.cpp src/cheat_algorithm.h)

add_library(cheatlib STATIC ${CHEAT_SOURCES})

add_executable(CheatServer src/cheat_server.cpp)

target_link_libraries(CheatServer pthread)
target_link_libraries(CheatServer tacopie)
target_link_libraries(CheatServer cpp_redis)
target_link_libraries(CheatServer mysqlcppconn)
target_link_libraries(CheatServer ${Boost_LIBRARIES})
target_link_libraries(CheatServer cheatlib)
