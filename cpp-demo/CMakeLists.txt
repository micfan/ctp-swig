cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/spdlog
    ${CTP_LIBRARY_DIRS}
)

link_directories(
    ${CTP_LIBRARY_DIRS}
)

add_subdirectory(md)
add_subdirectory(td)