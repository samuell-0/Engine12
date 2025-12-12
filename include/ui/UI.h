#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_vulkan.h>

#include "core/States.hpp"
#include "ui/IconsAwesome6.h"

#include "ui/Utils.h"
#include "UIController.h"

#include "debuging/Log.hpp"

namespace UI
{
    bool create_window(AppState* appstate);
    VkResult init_imgui(AppState* appstate);
    void shutdown_imgui(AppState* appstate);
    void draw_ui(AppState* appstate);
    void clean_up(AppState* appstate);
    void cmd_draw(AppState* appstate, uint32_t index);
    inline void draw_ui(AppState* appstate)
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        int w = 0, h = 0;
        SDL_GetWindowSizeInPixels(appstate->window, &w, &h);
        appstate->ui_data.window_width = (uint16_t)w;
        appstate->ui_data.window_height = (uint16_t)h;
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(appstate->ui_data.window_width, appstate->ui_data.window_height));
        if (ImGui::Begin("test window ig", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground))
        {
            ImGui::GetWindowDrawList()->AddLine(ImVec2((float)appstate->ui_data.V1, 0), ImVec2((float)appstate->ui_data.V1, (float)appstate->ui_data.window_height), IM_COL32(255, 255, 255, 255));
            ImGui::GetWindowDrawList()->AddLine(ImVec2((float)appstate->ui_data.V0, 0), ImVec2((float)appstate->ui_data.V0, (float)appstate->ui_data.H0), IM_COL32(255, 255, 255, 255));
            ImGui::GetWindowDrawList()->AddLine(ImVec2(0, (float)appstate->ui_data.H0), ImVec2((float)appstate->ui_data.V1, appstate->ui_data.H0), IM_COL32(255, 255, 255, 255));
            ImGui::GetWindowDrawList()->AddLine(ImVec2((float)appstate->ui_data.V1, (float)appstate->ui_data.H1), ImVec2((float)appstate->ui_data.window_width, appstate->ui_data.H1), IM_COL32(255, 255, 255, 255));
            ImGui::GetWindowDrawList()->AddLine(ImVec2((float)appstate->ui_data.V2, (float)appstate->ui_data.H0), ImVec2((float)appstate->ui_data.V2, appstate->ui_data.window_height), IM_COL32(255, 255, 255, 255));
            ImGui::GetWindowDrawList()->AddLine(ImVec2((float)appstate->ui_data.V3, (float)appstate->ui_data.H0), ImVec2((float)appstate->ui_data.V3, appstate->ui_data.window_height), IM_COL32(255, 255, 255, 255));
            int v0 = appstate->ui_data.V0;
            int v1 = appstate->ui_data.V1;
            int h0 = appstate->ui_data.H0;
            int v2 = appstate->ui_data.V2;
            int v3 = appstate->ui_data.V3;
            int h1 = appstate->ui_data.H1;
            if (ImGui::SliderInt("V0", &v0, 0, 2000))
            {
                UIController::resize_V0(v0);
            }
            if (ImGui::SliderInt("V1", &v1, 0, 2000))
            {
                UIController::resize_V1(v1);
            }
            if (ImGui::SliderInt("H0", &h0, 0, 2000))
            {
                UIController::resize_H0(h0);
            }
            if (ImGui::SliderInt("V2", &v2, 0, 2000))
            {
                UIController::resize_V2(v2);
            }
            if (ImGui::SliderInt("V3", &v3, 0, 2000))
            {
                UIController::resize_V3(v3);
            }
            if (ImGui::SliderInt("H1", &h1, 0, 2000))
            {
                UIController::resize_H1(h1);
            }
            ImGui::End();
        }
        ImGui::Render();
    }

}