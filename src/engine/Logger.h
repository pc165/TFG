#ifndef TFG_LOGGER_H
#define TFG_LOGGER_H

#include "spdlog/spdlog.h"

#define LOG_CALL(level, ...) (spdlog::default_logger_raw())->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)

#define LOG_TRACE(...) LOG_CALL(spdlog::level::trace,__VA_ARGS__)
#define LOG_DEBUG(...) LOG_CALL(spdlog::level::debug,__VA_ARGS__)
#define LOG_INFO(...) LOG_CALL(spdlog::level::info,__VA_ARGS__)
#define LOG_WARN(...) LOG_CALL(spdlog::level::warn,__VA_ARGS__)
#define LOG_ERROR(...) LOG_CALL(spdlog::level::err,__VA_ARGS__)
#define LOG_CRITICAL(...) LOG_CALL(spdlog::level::critical,__VA_ARGS__)


#endif //TFG_LOGGER_H
