cmake_minimum_required(VERSION 3.10)

project(obsidian)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_GENERATOR_PLATFORM x64)

#add_subdirectory(${PROJECT_SOURCE_DIR}/lib/minhook)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADER_FILES FOLLOW_SYMLINKS ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE SRC_FILES FOLLOW_SYMLINKS ${PROJECT_SOURCE_DIR}/src/*.c ${PROJECT_SOURCE_DIR}/src/*.cpp)

source_group(TREE "${PROJECT_SOURCE_DIR}" PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE "${PROJECT_SOURCE_DIR}" PREFIX "Source Files" FILES ${SRC_FILES})

add_library(obsidian SHARED ${HEADER_FILES} ${SRC_FILES})

#target_link_libraries(obsidian minhook)
target_include_directories(obsidian PUBLIC
  #"${PROJECT_SOURCE_DIR}/lib/minhook/include"
  "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_COMMAND "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../../flintandsteel/$<CONFIG>/flintandsteel.exe"
        VS_DEBUGGER_COMMAND_ARGUMENTS "\"Minecraft 1.19.2 - Singleplayer\" \"$<TARGET_FILE:${PROJECT_NAME}>\""
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
