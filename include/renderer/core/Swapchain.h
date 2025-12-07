#pragma once
#include "core/States.hpp"

namespace Swapchain
{
    VkResult create_swapchain(AppState* appstate);
    VkResult create_framebuffers(AppState* appstate);
    VkResult recreate_swapchain(AppState* appstate);
}