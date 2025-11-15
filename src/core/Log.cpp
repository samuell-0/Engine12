#include "core/Log.hpp"

std::vector<LogEntry> Log::entries;
#include "iostream"
VkResult Log::push(LogLevel lvl, const std::string& msg, VkResult res) {
    entries.push_back({lvl, msg, res});
    for (auto i: entries)
        std::cout << i.message << std::endl;
    return res;
}
