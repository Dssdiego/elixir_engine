//
// Created by Diego S. Seabra on 14/03/22.
//

#ifndef VULKAN_ENGINE_LOGGER_H
#define VULKAN_ENGINE_LOGGER_H

#define SHOW_CONSOLE

#include <iostream>
#include <fstream>

#include <iomanip>
#include <ctime>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#endif

#if __linux__ || __APPLE__
#define RESET "\e[0m"
#define COLOR_BLACK "\e[0;30m"
#define COLOR_GRAY "\e[0;37m"
#define COLOR_RED "\e[0;91m"
#define COLOR_GREEN "\e[0;92m"
#define COLOR_YELLOW "\e[0;93m"
#define COLOR_BLUE "\e[0;94m"
#define COLOR_PURPLE "\e[0;95m"
#define COLOR_CYAN "\e[0;96m"
#define COLOR_WHITE "\e[0;97m"
#else
#define COLOR_GRAY 8
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_BLUE 9
#define COLOR_PURPLE 13
#define COLOR_CYAN 11
#define COLOR_WHITE 7
#endif

const std::string INFO_STR = "INFO::";
const std::string WARN_STR = "WARN::";
const std::string ERROR_STR = "ERROR::";
const std::string DEBUG_STR = "DEBUG::";

struct CLoggerImpl
{
//    CLoggerImpl();
//    ~CLoggerImpl();
};

// TODO: Save log to file (when enabled by the engine)

class CLogger
{
public:
    static void Info(std::string msg)
    {
#ifdef SHOW_CONSOLE
#if __linux__ || __APPLE__
        std::cout << COLOR_BLUE << INFO_STR << msg << RESET << std::endl;
#else
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_BLUE);
        std::cout << INFO_STR << msg << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
#endif
//        WriteToLogFile(INFO_STR, msg);
#endif
    }

    static void Warn(std::string msg)
    {
#ifdef SHOW_CONSOLE
#if __linux__ || __APPLE__
        std::cout << COLOR_YELLOW << WARN_STR << msg << RESET << std::endl;
#else
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_YELLOW);
        std::cout << WARN_STR << msg << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
#endif
//        WriteToLogFile(WARN_STR, msg);
#endif
    }

    static void Error(std::string errMsg, char* errParam)
    {
#ifdef SHOW_CONSOLE
#if __linux__ || __APPLE__
        std::cout << COLOR_RED << ERROR_STR << errMsg << " -> " << errParam << RESET << std::endl;
#else
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
        std::cout << ERROR_STR << errMsg << " -> " << errParam << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
#endif
//        WriteToLogFile(ERROR_STR, errMsg);
#endif
    }

    static void Debug(std::string msg)
    {
#ifdef SHOW_CONSOLE
#if __linux__ || __APPLE__
        std::cout << COLOR_PURPLE << DEBUG_STR << msg << RESET << std::endl;
#else
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_PURPLE);
        std::cout << DEBUG_STR << msg << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
#endif
//        WriteToLogFile(DEBUG_STR, msg);
#endif
    }
};


#endif //VULKAN_ENGINE_LOGGER_H
