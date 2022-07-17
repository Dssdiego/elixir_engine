//
// Created by Diego S. Seabra on 16/07/22.
//

#ifndef VULKAN_ENGINE_CONFIG_H
#define VULKAN_ENGINE_CONFIG_H

#include <inih/INIReader.h>
#include <iostream>

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
    static std::string GetEngineName();
    static std::string GetEngineVersion();

private:
    INIReader reader;

    Config();

    uint32_t GetWindowWidthImpl();
    uint32_t GetWindowHeightImpl();
    uint32_t GetWindowPositionXImpl();
    uint32_t GetWindowPositionYImpl();
    bool GetSaveToLogFileImpl();
    std::string GetEngineNameImpl();
    std::string GetEngineVersionImpl();
};


#endif //VULKAN_ENGINE_CONFIG_H
