#pragma once
#include "core/State.hpp"

namespace CommandPool{
    VkResult create_command_pool(AppState* appstate);
    VkResult create_command_buffers(AppState* appstate);
    VkCommandBuffer begin_single_time_command(AppState* as);
    VkResult end_single_time_command(AppState* as, VkCommandBuffer commandBuffer);
}