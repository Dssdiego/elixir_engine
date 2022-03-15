cmake_minimum_required(VERSION 3.20)
project(vulkan_engine)

set(CMAKE_CXX_STANDARD 17)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/lua)

# Vulkan
find_package(Vulkan REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# FMOD
set(FMOD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/fmod/inc")
set(FMOD_LIBRARY
        "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmod_vc.lib" # for production use
        "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmodstudio_vc.lib" # for production use
        #            "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmodstudioL_vc.lib" # for debug use
        )
file(COPY
        "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmod.dll" # for production use
        "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmodstudio.dll" # for production use
        #            "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmodstudioL.dll" # for debug use
        DESTINATION ${CMAKE_BINARY_DIR})

# Messages
message("Libraries:")
message(VULKAN_INCLUDE_DIRS: ${Vulkan_INCLUDE_DIRS})
message(VULKAN_LIB: ${Vulkan_LIBRARIES})
message(OPENGL_LIBRARIES: ${OPENGL_LIBRARIES})

# include directories
include_directories(
        thirdparty
        ${FMOD_INCLUDE_DIR}
)

# load source and header files
file(GLOB_RECURSE SOURCE_FILES
        "src/*.h"
        "src/*.cpp")

add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
        thirdparty/glad/glad.c
        )

if(WIN32)
    target_link_libraries(
            ${PROJECT_NAME}
            glfw
            Vulkan::Vulkan
            d3d12
            OpenGL::GL
            lua_lib
            ${FMOD_LIBRARY}
    )
else()
    target_link_libraries(
            ${PROJECT_NAME}
            glfw
            Vulkan::Vulkan
            OpenGL::GL
    )
endif()

# TODO: Package project with CPACK