cmake_minimum_required(VERSION 3.7)

project(wolf-shaper)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(dpf)
add_subdirectory(plugins/wolf-shaper)
