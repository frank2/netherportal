cmake_minimum_required(VERSION 3.10)

project(flintandsteel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_definitions( -D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING )

# add_subdirectory(${PROJECT_SOURCE_DIR}/lib/keystone)

# include_directories(${PROJECT_SOURCE_DIR}/lib/keystone/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADER_FILES FOLLOW_SYMLINKS ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE SRC_FILES FOLLOW_SYMLINKS ${PROJECT_SOURCE_DIR}/src/*.c ${PROJECT_SOURCE_DIR}/src/*.cpp)

source_group(TREE "${PROJECT_SOURCE_DIR}" PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE "${PROJECT_SOURCE_DIR}" PREFIX "Source Files" FILES ${SRC_FILES})

add_executable(flintandsteel ${HEADER_FILES} ${SRC_FILES})

# target_link_libraries(flintandsteel keystone)
target_include_directories(flintandsteel PUBLIC
  #"${PROJECT_SOURCE_DIR}/lib/keystone/include"
  "${PROJECT_SOURCE_DIR}/include"
)
