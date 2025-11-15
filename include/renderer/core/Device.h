#pragma once
#include "core/State.hpp"
namespace Device{
    VkResult device_initialization(AppState* appstate);
    VkResult get_queues(AppState* appstate);
}