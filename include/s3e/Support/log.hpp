#pragma once

#include <exception>
#include <memory>
#include <string>

#ifndef SPDLOG_ACTIVE_LEVEL
#ifdef NDEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif
#endif

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Tom::s3e::Logging {
    class Logger {
    public:
        enum Level {
            Trace,
            Debug,
            Info,
            Warning,
            Error,
            Critical,
        };

    private:
        std::shared_ptr<spdlog::logger> logger;
        Level level;

    public:
        typedef std::shared_ptr<Logger> Ptr;
        typedef std::shared_ptr<const Logger> ConstPtr;

        template<typename FormatString>
        Logger(const FormatString & subsystem)
            : logger(spdlog::stdout_color_mt(subsystem)) {}

        template<typename FormatString, typename... Args>
        void trace(const FormatString & fmt, Args &&... args) {
            logger->trace(fmt, std::forward<Args>(args)...);
        }

        template<typename FormatString, typename... Args>
        void debug(const FormatString & fmt, Args &&... args) {
            logger->debug(fmt, std::forward<Args>(args)...);
        }

        template<typename FormatString, typename... Args>
        void info(const FormatString & fmt, Args &&... args) {
            logger->info(fmt, std::forward<Args>(args)...);
        }

        template<typename FormatString, typename... Args>
        void warning(const FormatString & fmt, Args &&... args) {
            logger->warn(fmt, std::forward<Args>(args)...);
        }

        template<typename FormatString, typename... Args>
        void error(const FormatString & fmt, Args &&... args) {
            logger->error(fmt, std::forward<Args>(args)...);
        }

        template<typename FormatString, typename... Args>
        [[noreturn]] void critical(const FormatString & fmt,
                                   Args &&... args) noexcept {
            logger->critical(fmt, std::forward<Args>(args)...);
            std::terminate();
        }

        void setLevel(Level level) {
            this->level = level;
            switch (level) {
                case Trace:
                    logger->set_level(spdlog::level::trace);
                    break;
                case Debug:
                    logger->set_level(spdlog::level::debug);
                    break;
                case Info:
                    logger->set_level(spdlog::level::info);
                    break;
                case Warning:
                    logger->set_level(spdlog::level::warn);
                    break;
                case Error:
                    logger->set_level(spdlog::level::err);
                    break;
                case Critical:
                    logger->set_level(spdlog::level::critical);
                    break;
            }
        }
    };

    extern Logger::Ptr Support;
    extern Logger::Ptr Graphics;
    extern Logger::Ptr Core;
}