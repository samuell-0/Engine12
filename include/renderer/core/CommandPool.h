#pragma once
#include "window/State.hpp"

namespace CommandPool{
    bool create_command_pool(AppState* appstate);
    bool create_command_buffers(AppState* appstate);
}