#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_vulkan.h>

#include "core/State.hpp"
#include "ui/IconsAwesome6.h"

#include "core/Log.hpp"

inline void draw_test_setting_window(AppState* appstate){
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(10, 10, 10, 230));

    ImGui::BeginChild("##TestWindow", ImVec2(0.0f, 0.0f), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 27.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100);

    if (ImGui::Button(ICON_FA_DOWNLOAD, ImVec2(25.0f, 25.0f)))
        appstate->ui_data.which_setting = SettingOpened::None;

    ImGui::PopStyleVar();
    ImGui::EndChild();

    ImGui::PopStyleColor();
}

inline void draw_left_panel(AppState* appstate){
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(10, 10, 10, 255));
    ImGui::BeginChild("##LeftPanel", ImVec2(0.0f, appstate->ui_data.window_hight), 0);
    if (ImGui::BeginTabBar("##TabBar")){

        bool api_tab_is_open = ImGui::BeginTabItem("API");
        if (ImGui::IsItemHovered()){
            ImGui::BeginTooltip();
            ImGui::Text("API specific details");
            ImGui::EndTooltip();
        }
        if (api_tab_is_open){

            if (ImGui::Button("test", ImVec2(ImGui::GetWindowWidth(), 0.0f)))
                if (appstate->ui_data.which_setting != SettingOpened::Test)
                    appstate->ui_data.which_setting = SettingOpened::Test;
                else
                    appstate->ui_data.which_setting = SettingOpened::None;
            if (ImGui::Button("ubnlk")){
                ImGui::OpenPopup("pop");
            }
            if (ImGui::BeginPopup("pop")){
                ImGui::ShowMetricsWindow();
                ImGui::EndPopup();
            }
            ImGui::EndTabItem();
        }
        bool simulation_tab_is_open = ImGui::BeginTabItem("Simulation");
        if (ImGui::IsItemHovered()){
            ImGui::BeginTooltip();
            ImGui::Text("Customize the simulation");
            ImGui::EndTooltip();
        }
        if (simulation_tab_is_open){
            if (ImGui::Button("ubnlk")){
                ImGui::OpenPopup("pop");
            }
            if (ImGui::BeginPopup("pop")){
                ImGui::ColorPicker4("clear", appstate->clearColor.color.float32);
                ImGui::EndPopup();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

inline void draw_view_window(AppState* appstate){
    ImGui::BeginChild("##ViewWindow", ImVec2(0.0f, 0.0f), ImGuiChildFlags_ResizeY, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoScrollbar);
    switch (appstate->ui_data.which_setting)
    {
        case SettingOpened::Test:
            draw_test_setting_window(appstate);
    }
    ImGui::EndChild();
}

inline void draw_buttom_panel(AppState* appstate){
    ImGui::BeginChild("##BottomPanel", ImVec2(0, 0), 0, ImGuiWindowFlags_NoDecoration);
        if (ImGui::BeginTable("##BottomTable", 2, ImGuiTableFlags_Resizable)){

            ImGui::TableNextRow();
            
            ImGui::TableNextColumn();

            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(16, 28, 33, 255));
            ImGui::BeginChild("##LeftColumn", ImVec2(0, 0), 0);
                ImGui::EndChild();
            ImGui::PopStyleColor();

            ImGui::TableNextColumn();

            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(10, 10, 10, 255));
            ImGui::BeginChild("##RightColumn", ImVec2(0, 0), 0);
                ImGui::EndChild();
    
            ImGui::PopStyleColor();

            ImGui::EndTable();
        }
    ImGui::EndChild();
}

inline void draw_visualization_panel(AppState* appstate){
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(16, 28, 33, 255));

    ImGui::BeginChild("##Visualization", ImVec2(0, 0), ImGuiChildFlags_ResizeY | 0);
    ImGui::EndChild();

    ImGui::PopStyleColor();
}

inline void draw_editor_panel(AppState* appstate){
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(10, 10, 10, 255));

    ImGui::BeginChild("##EditorPanel", ImVec2(0, 0), 0);
    ImGui::EndChild();

    ImGui::PopStyleColor();
}

inline void draw_main_menu_bar(AppState* appstate){
    ImGui::BeginChild("##MainMenuBar", ImVec2(0, 1), 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0));
    if (ImGui::BeginMenuBar()){
        float width = ImGui::GetWindowWidth();
        ImGui::MenuItem(ICON_FA_FILE " New"); ImGui::Separator();
        ImGui::MenuItem(ICON_FA_FOLDER " Open"); ImGui::Separator();
        ImGui::MenuItem(ICON_FA_SAVE " Save"); ImGui::Separator();
        if (ImGui::BeginMenu(ICON_FA_SAVE " jnmo")){
            ImGui::ShowDebugLogWindow();
            // ImGui::ShowStyleEditor();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    ImGui::PopStyleVar();
    ImGui::EndChild();
}

inline void draw_secondary_menu_bar(AppState* appstate){
    ImGui::BeginChild("##SecondaryMenuBar", ImVec2(0, 1), 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0));
    if (ImGui::BeginMenuBar()){
        float width = ImGui::GetWindowWidth();
        ImGui::MenuItem("Item 1"); ImGui::Separator();
        ImGui::MenuItem("Item 2"); ImGui::Separator();
        ImGui::MenuItem("Item 3"); ImGui::Separator();

        ImGui::EndMenuBar();
    }
    ImGui::PopStyleVar();
    ImGui::EndChild();
}

namespace UI{
    bool create_window(AppState* appstate);
    VkResult init_imgui(AppState* appstate);
    void shutdown_imgui(AppState* appstate);
    void draw_ui(AppState* appstate);
    void clean_up(AppState* appstate);
    inline void draw_ui(AppState* appstate){

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        

        ImGui::SetNextWindowPos(ImVec2(0, 0));

        ImGui::SetNextWindowSize(ImVec2(appstate->ui_data.window_width, appstate->ui_data.window_hight));
        ImGui::Begin("##FullscreenOverlay", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoBringToFrontOnFocus);

        if (ImGui::BeginTable("##MainLayout", 3, ImGuiTableFlags_Resizable)){
            ImGui::TableNextRow();

            ImGui::TableNextColumn(); draw_left_panel(appstate);
            ImGui::TableNextColumn(); draw_main_menu_bar(appstate); draw_view_window(appstate); draw_buttom_panel(appstate);
            ImGui::TableNextColumn(); draw_secondary_menu_bar(appstate); draw_visualization_panel(appstate); draw_editor_panel(appstate);
            ImGui::EndTable();
        }
        ImGui::End();

        ImGui::Render();
    }

}