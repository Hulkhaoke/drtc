#ifndef _LOG_H_
#define _LOG_H_

#include "spdlog/spdlog.h"
#include "spdlog/common.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"


#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std::chrono;

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO

// SPDLOG_DEBUG(...)
// SPDLOG_INFO(...)
// SPDLOG_WARN(...)
// SPDLOG_ERROR(...)
// SPDLOG_CRITICAL(...)

constexpr auto LOGGER_NAME = "rtc";
constexpr auto LOGGER_SAVE_PATH = "logs/rtc-log";
#define SPDLOG_INFO_FILE(...)                                                                             \
    auto logger0 = spdlog::get(LOGGER_NAME);                                                              \
    if (nullptr == logger0)                                                                               \
    {                                                                                                     \
        auto now = std::chrono::system_clock::now() +  std::chrono::hours(8);                             \
        auto timet = std::chrono::system_clock::to_time_t(now);                                           \
        auto localTime = *std::gmtime(&timet);                                                            \
        std::stringstream ss;                                                                             \
        std::string str;                                                                                  \
        ss << std::put_time(&localTime, "%Y%m%d-%H%M%S");                                                 \
        ss >> str;                                                                                        \
        std::cout << str << std::endl;                                                                    \
        std::string filename = "logs/rtc-log-" + str + ".log";                                            \
        std::vector<spdlog::sink_ptr> sinks;                                                              \
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());                         \
        sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename, 1048576*5, 3));  \
        auto combined_logger = std::make_shared<spdlog::logger>(LOGGER_NAME, begin(sinks), end(sinks));   \
        spdlog::register_logger(combined_logger);                                                         \
        SPDLOG_LOGGER_INFO(combined_logger, __VA_ARGS__);                                                 \
    }                                                                                                     \
    else                                                                                                  \
    {                                                                                                     \
        SPDLOG_LOGGER_INFO(logger0, __VA_ARGS__);                                                         \
    }

#define SPDLOG_WARN_FILE(...)                                                                             \
    auto logger1 = spdlog::get(LOGGER_NAME);                                                              \
    if (nullptr == logger1)                                                                               \
    {                                                                                                     \
        auto rotating_logger = spdlog::rotating_logger_mt(LOGGER_NAME, LOGGER_SAVE_PATH, 1048576 * 5, 3); \
        SPDLOG_LOGGER_WARN(rotating_logger, __VA_ARGS__);                                                 \
    }                                                                                                     \
    else                                                                                                  \
    {                                                                                                     \
        SPDLOG_LOGGER_WARN(logger1, __VA_ARGS__);                                                         \
    }

#define SPDLOG_ERROR_FILE(...)                                                                            \
    auto logger2 = spdlog::get(LOGGER_NAME);                                                              \
    if (nullptr == logger2)                                                                               \
    {                                                                                                     \
        auto rotating_logger = spdlog::rotating_logger_mt(LOGGER_NAME, LOGGER_SAVE_PATH, 1048576 * 5, 3); \
        SPDLOG_LOGGER_ERROR(rotating_logger, __VA_ARGS__);                                                \
    }                                                                                                     \
    else                                                                                                  \
    {                                                                                                     \
        SPDLOG_LOGGER_ERROR(logger2, __VA_ARGS__);                                                        \
    }

#define SPDLOG_CRITICAL_FILE(...)                                                                         \
    auto logger3 = spdlog::get(LOGGER_NAME);                                                              \
    if (nullptr == logger3)                                                                               \
    {                                                                                                     \
        auto rotating_logger = spdlog::rotating_logger_mt(LOGGER_NAME, LOGGER_SAVE_PATH, 1048576 * 5, 3); \
        SPDLOG_LOGGER_CRITICAL(rotating_logger, __VA_ARGS__);                                             \
    }                                                                                                     \
    else                                                                                                  \
    {                                                                                                     \
        SPDLOG_LOGGER_CRITICAL(logger3, __VA_ARGS__);                                                     \
    }

#endif
