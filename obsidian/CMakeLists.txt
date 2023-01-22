cmake_minimum_required(VERSION 3.10)

project(obsidian)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_GENERATOR_PLATFORM x64)
add_definitions( -D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING )

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADER_FILES FOLLOW_SYMLINKS ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE SRC_FILES FOLLOW_SYMLINKS ${PROJECT_SOURCE_DIR}/src/*.c ${PROJECT_SOURCE_DIR}/src/*.cpp)

source_group(TREE "${PROJECT_SOURCE_DIR}" PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE "${PROJECT_SOURCE_DIR}" PREFIX "Source Files" FILES ${SRC_FILES})

add_library(obsidian SHARED ${HEADER_FILES} ${SRC_FILES})
add_dependencies(obsidian flintandsteel easyloggingpp nlohmann_json)

#target_link_libraries(obsidian minhook)

target_link_directories(obsidian PUBLIC
  "$ENV{JAVA_HOME}/lib"
)
target_link_libraries(obsidian PRIVATE urlmon easyloggingpp jvm jawt nlohmann_json::nlohmann_json)
target_include_directories(obsidian PUBLIC
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/../lib/easyloggingpp/src"
  "${PROJECT_SOURCE_DIR}/../lib/json/include"
  "$ENV{JAVA_HOME}/include"
  "$ENV{JAVA_HOME}/include/win32"
  #"${PROJECT_SOURCE_DIR}/../lib/minhook/include"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_COMMAND "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../../flintandsteel/$<CONFIG>/flintandsteel.exe"
        VS_DEBUGGER_COMMAND_ARGUMENTS "--payload \"$<TARGET_FILE:${PROJECT_NAME}>\" --window-title \"Minecraft 1.19.2 - Singleplayer\""
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
