cmake_minimum_required(VERSION 3.10)

project(netherportal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_BUILD_TYPE "Debug")

set(build_static_lib ON CACHE BOOL "Build easylogging as a static library.")
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/easyloggingpp)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/argparse)

add_subdirectory(flintandsteel)
add_subdirectory(obsidian)
