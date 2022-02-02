cmake_minimum_required(VERSION 3.20)
project(vulkan_engine)

set(CMAKE_CXX_STANDARD 17)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/glfw)

# include directories
include_directories(
        thirdparty
)

# load source and header files
file(GLOB_RECURSE SOURCE_FILES
        "src/*.h"
        "src/*.cpp")

add_executable(${PROJECT_NAME} WIN32
        ${SOURCE_FILES}
        )

target_link_libraries(
        ${PROJECT_NAME}
        glfw
)