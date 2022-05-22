#include "Logger.h"

void InitLogger() {
    spdlog::set_pattern("[%H:%M:%S.%e] [%l] [%s:%#:%!] %v");
    spdlog::set_level(spdlog::level::trace);
}