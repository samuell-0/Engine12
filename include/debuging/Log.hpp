#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

enum class LogLevel { Info, Warning, Error };

struct LogEntry
{
    LogLevel level;
    std::string message;
    VkResult vk_result;
};

class Log
{
    // private:
    //     static std::vector<LogEntry> entries;
    public:
        static std::vector<LogEntry> entries;
        static VkResult push(LogLevel lvl, const std::string& msg, VkResult res = VK_SUCCESS);
};
