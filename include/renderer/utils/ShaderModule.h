#pragma once
#include "window/State.hpp"

namespace ShaderModule{
    bool           compile_shader();
    VkShaderModule create_shader_module(AppState* appstate, const char* path_to_glsl);
}
