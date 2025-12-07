#pragma once
#include "core/States.hpp"

namespace CommandPool
{
    VkResult create_command_pool(AppState* appstate);
    VkResult allocate_command_buffers(AppState* appstate);
    VkResult record_command_buffer(AppState* appstate, uint32_t image_index);
    VkCommandBuffer begin_single_time_command(AppState* as);
    VkResult end_single_time_command(AppState* as, VkCommandBuffer commandBuffer);
}