cmake_minimum_required(VERSION 3.20)
project(vulkan_engine)

set(CMAKE_CXX_STANDARD 17)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/lua)
add_subdirectory(thirdparty/imgui)
add_subdirectory(thirdparty/tracy)

# Vulkan
find_package(Vulkan REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# FMOD
set(FMOD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/fmod/inc")

if(WIN32)
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
else()
    set(FMOD_LIBRARY
            "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/libfmod.so" # for production use
            "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/libfmodstudio.so" # for production use
            #            "${CMAKE_SOURCE_DIR}/thirdparty/fmod/lib/x64/fmodstudioL_vc.lib" # for debug use
            )
endif()

# Geforce NOW
set(GEFORCE_NOW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/geforce_now/include")
#set(GEFORCE_NOW_LIBRARY
#        "${CMAKE_SOURCE_DIR}/thirdparty/geforce_now/lib/x64/GfnRuntimeSdk.dll"
#        )
file(COPY
        "${CMAKE_SOURCE_DIR}/thirdparty/geforce_now/lib/x64/GfnRuntimeSdk.dll"
        DESTINATION ${CMAKE_BINARY_DIR})
#add_library(GEFORCE_NOW_LIBRARY SHARED thirdparty/geforce_now/lib/x64/GfnRuntimeSdk.dll)
#set_target_properties(GEFORCE_NOW_LIBRARY PROPERTIES LINKER_LANGUAGE C)

# Messages
message("Libraries:")
message(VULKAN_INCLUDE_DIRS: ${Vulkan_INCLUDE_DIRS})
message(VULKAN_LIB: ${Vulkan_LIBRARIES})
message(OPENGL_LIBRARIES: ${OPENGL_LIBRARIES})
message(FMOD_LIBRARY: ${FMOD_LIBRARY})
message(GEFORCE_NOW_LIBRARY: ${GEFORCE_NOW_LIBRARY})

# include directories
include_directories(
        thirdparty
        ${FMOD_INCLUDE_DIR}
        ${GEFORCE_NOW_INCLUDE_DIR}
)

# load source and header files
file(GLOB_RECURSE SOURCE_FILES
        "src/*.h"
        "src/*.cpp")

# Package project with CPACK
SET(CPACK_GENERATOR "ZIP")
SET(CPACK_PACKAGE_NAME "Elixir Engine")
include(CPack)

# application icon
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

# creating executable
add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
        thirdparty/glad/glad.c
        ${APP_ICON_RESOURCE_WINDOWS}
        )

if(WIN32)
    target_link_libraries(
            ${PROJECT_NAME}
            glfw
            Vulkan::Vulkan
            d3d12
            OpenGL::GL
            lua_lib
            imgui
            TracyClient
            ${FMOD_LIBRARY}
    )
else()
    target_link_libraries(
            ${PROJECT_NAME}
            glfw
            Vulkan::Vulkan
            OpenGL::GL
            lua_lib
            imgui
            TracyClient
            ${FMOD_LIBRARY}
    )
endif()

# defining final executable name
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Elixir Engine")

# asset copying
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
        )
add_dependencies(${PROJECT_NAME} copy_assets)

# TODO: Use rcedit after cmake build to change windows executable details
# TODO: Package project with CPACK