#pragma once
#include "window/State.hpp"

namespace Swapchain{
    bool create_swapchain(AppState* appstate);
    bool create_framebuffers(AppState* appstate);
    bool recreate_swapchain(AppState* appstate);
}