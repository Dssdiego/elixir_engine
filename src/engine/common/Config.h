//
// Created by Diego S. Seabra on 16/07/22.
//

#ifndef VULKAN_ENGINE_CONFIG_H
#define VULKAN_ENGINE_CONFIG_H

#include <inih/INIReader.h>
#include <iostream>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define ENGINE_NAME "Elixir Engine"

#define ENGINE_VERSION_MAJOR 0
#define ENGINE_VERSION_MINOR 1
#define ENGINE_VERSION_PATCH 0
#define RELEASE_TYPE "alpha"

#define ENGINE_VERSION "v" STR(ENGINE_VERSION_MAJOR) \
    "." STR(ENGINE_VERSION_MINOR) \
    "." STR(ENGINE_VERSION_PATCH) \
    "-" RELEASE_TYPE

// Config singleton
class Config
{
public:
    // no copy allowed!
    Config(const Config*) = delete;

    // singleton instance
    static Config& GetSingleton();

    // methods
    static uint32_t GetWindowWidth();
    static uint32_t GetWindowHeight();
    static uint32_t GetWindowPositionX();
    static uint32_t GetWindowPositionY();
    static bool GetSaveToLogFile();

private:
    INIReader reader;

    Config();

    uint32_t GetWindowWidthImpl();
    uint32_t GetWindowHeightImpl();
    uint32_t GetWindowPositionXImpl();
    uint32_t GetWindowPositionYImpl();
    bool GetSaveToLogFileImpl();
};


#endif //VULKAN_ENGINE_CONFIG_H
