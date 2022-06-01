//
// Created by Diego S. Seabra on 29/05/22.
//

#include "Logger.h"
#include "../config.h"

/*
 * INITIALIZATION
 */
void CLogger::Init()
{
    // REVIEW: This could be leaking memory!
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y_%m_%d");
    auto dateStr = oss.str();

    std::string buf(ENGINE_NAME);
    buf.append(" ");
    buf.append(ENGINE_VERSION);

    std::ofstream logFile;
    logFile.open("elixir_" + dateStr + std::string(".log"));
    logFile.clear();
    logFile << buf << "\n\n";
}

/*
 * INFO
 */
void CLogger::Info(std::string msg)
{
#ifdef SHOW_CONSOLE
#if __linux__ || __APPLE__
    std::cout << COLOR_BLUE << INFO_STR << msg << RESET << std::endl;
#else
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_BLUE);
    std::cout << INFO_STR << msg << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
#endif
    WriteToLogFile(INFO_STR, msg);
#endif
}

/*
 * WARNINGS
 */
void CLogger::Warn(std::string msg)
{
#ifdef SHOW_CONSOLE
#if __linux__ || __APPLE__
    std::cout << COLOR_YELLOW << WARN_STR << msg << RESET << std::endl;
#else
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_YELLOW);
    std::cout << WARN_STR << msg << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
#endif
    WriteToLogFile(WARN_STR, msg);
#endif
}

/*
 * ERRORS
 */
void CLogger::Error(std::string errMsg, char *errParam)
{
#ifdef SHOW_CONSOLE
#if __linux__ || __APPLE__
    std::cout << COLOR_RED << ERROR_STR << errMsg << " -> " << errParam << RESET << std::endl;
#else
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
    std::cout << ERROR_STR << errMsg << " -> " << errParam << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
#endif
    WriteToLogFile(ERROR_STR, errMsg);
#endif
}

/*
 * DEBUG
 */
void CLogger::Debug(std::string msg)
{
#ifdef SHOW_CONSOLE
#if __linux__ || __APPLE__
    std::cout << COLOR_PURPLE << DEBUG_STR << msg << RESET << std::endl;
#else
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_PURPLE);
    std::cout << DEBUG_STR << msg << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
#endif
    WriteToLogFile(DEBUG_STR, msg);
#endif
}

/*
 * WRITE TO FILE
 */
void CLogger::WriteToLogFile(std::string type, std::string msg)
{
    // REVIEW: This could be leaking memory!
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    auto timeStr = oss.str();

    std::ostringstream ossDate;
    ossDate << std::put_time(&tm, "%Y_%m_%d");
    auto dateStr = ossDate.str();

    std::string buf("");
    buf.append(timeStr);
    buf.append("\t");
    buf.append(type);
    buf.append(msg);

    std::ofstream logFile;
    logFile.open("elixir_" + dateStr + std::string(".log"), std::ios_base::app);
    logFile << buf << "\n";
}