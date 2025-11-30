#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_vulkan.h>

#include "core/State.hpp"
#include "ui/IconsAwesome6.h"

#include "core/Log.hpp"

inline void draw_main_menu_bar(AppState* appstate){
    ImGui::BeginChild("##MainMenuBar", ImVec2(0, 1), 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 0.0f));
    if (ImGui::BeginMenuBar()){
        float width = ImGui::GetWindowWidth();
        ImGui::MenuItem(ICON_FA_FILE " New"); ImGui::Separator();
        ImGui::MenuItem(ICON_FA_FOLDER " Open"); ImGui::Separator();
        ImGui::MenuItem(ICON_FA_SAVE " Save"); ImGui::Separator();
        if (ImGui::BeginMenu(ICON_FA_SAVE " jniubkmo")){
            ImGui::ShowDebugLogWindow();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ICON_FA_SAVE " jnmo")){
            ImGui::ColorPicker4("sz", appstate->clearColor.color.float32);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ICON_FA_SAVE " jnkjunmo")){
            ImGui::ShowMetricsWindow();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::PopStyleVar();
    ImGui::EndChild();
}

inline void draw_secondary_menu_bar(AppState* appstate){
    ImGui::BeginChild("##SecondaryMenuBar", ImVec2(0, 1), 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 0.0f));
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

inline void draw_window_ctrl_bar(AppState* appstate){
    float width   = ImGui::GetWindowWidth();
    if (width <= 100) return;
    ImVec2 start  = ImGui::GetWindowPos();

    if (ImGui::IsMouseHoveringRect(ImVec2(start.x + width * 0.35f - 3.0f, start.y), ImVec2(start.x + width * 0.65f + 3.0f, start.y + 25.0f + 3.0f))){
        ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(20, 20, 20, 250));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 7.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 40.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));

        ImGui::SetNextWindowPos(ImVec2(start.x + width * 0.35f, start.y));
        ImGui::SetNextWindowSize(ImVec2(width * 0.3, 25.0f));
        ImGui::BeginChild("##WindowCtrlBar", ImVec2(0, 0), 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);

        float spacing = (ImGui::GetWindowWidth() - 75) / 2;
        // NOTE: static assuming there is no other way in the app to change it
        static bool fullscreen = (SDL_GetWindowFlags(appstate->window) & SDL_WINDOW_FULLSCREEN) != 0;

        if (ImGui::Button(ICON_FA_FOLDER "##iu", ImVec2(25.0f, 25.0f)))
            0;

        ImGui::SameLine(0.0f, spacing);
        if (fullscreen){
            if (ImGui::Button(ICON_FA_FOLDER "##h", ImVec2(25.0f, 25.0f))){
                if (!SDL_SetWindowFullscreen(appstate->window, false)){
                    Log::push(LogLevel::Warning, "could not turn off fullscreen: " + std::string(SDL_GetError()));
                }
                else fullscreen = false;
            }
        }
        else{
            if (ImGui::Button(ICON_FA_FOLDER "##hkh", ImVec2(25.0f, 25.0f))){
                if (!SDL_SetWindowFullscreen(appstate->window, true)){
                    Log::push(LogLevel::Warning, "could not turn on fullscreen: " + std::string(SDL_GetError()));
                }
                else fullscreen = true;
            }
        }

        ImGui::SameLine(0.0f, spacing);
        if (ImGui::Button(ICON_FA_FOLDER "##kmjk", ImVec2(25.0f, 25.0f))){
            SDL_Event quit_event;
            quit_event.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&quit_event);
        }
        ImGui::EndChild();
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor();
    }
}

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

        bool api_tab_is_open = ImGui::BeginTabItem("Vulkan");
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
            {
                if (ImGui::Button("ubnlk")){
                    ImGui::OpenPopup("pop");
                }
                if (ImGui::BeginPopup("pop")){
                    ImGui::ShowMetricsWindow();
                    ImGui::EndPopup();
                }                
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
            // int count;
            // SDL_DisplayID display_id = SDL_GetDisplayForWindow(appstate->window);
            // SDL_DisplayMode** display_modes = SDL_GetFullscreenDisplayModes(display_id, &count);
            // for (int i = 0; i < count; i++){
            //     SDL_DisplayMode* display_mode = display_modes[i];
            //     char lable[32];
            //     sprintf(lable, "set to display mode id: %d", i);
            //     if (ImGui::Button(lable))
            //         if (!SDL_SetWindowFullscreenMode(appstate->window, display_mode))
            //             exit;
            // }
            static bool fs{false};
            if (ImGui::Button("fs")){
                if (!SDL_SetWindowFullscreen(appstate->window, fs)){
                    exit;
                }
                fs = !fs;
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
        default:
            draw_window_ctrl_bar(appstate);
    }
    ImGui::EndChild();
}

inline void draw_buttom_panel(AppState* appstate){
    ImGui::BeginChild("##BottomPanel", ImVec2(0.0f, 0.0f), 0, ImGuiWindowFlags_NoDecoration);
        if (ImGui::BeginTable("##BottomTable", 2, ImGuiTableFlags_Resizable)){

            ImGui::TableNextRow();
            
            ImGui::TableNextColumn();

            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(16, 28, 33, 255));
            ImGui::BeginChild("##LeftColumn", ImVec2(0.0f, 0.0f), 0);
                ImGui::EndChild();
            ImGui::PopStyleColor();

            ImGui::TableNextColumn();

            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(10, 10, 10, 255));
            ImGui::BeginChild("##RightColumn", ImVec2(0.0f, 0.0f), 0);
                ImGui::EndChild();
    
            ImGui::PopStyleColor();

            ImGui::EndTable();
        }
    ImGui::EndChild();
}

inline void draw_visualization_panel(AppState* appstate){
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(16, 28, 33, 255));

    ImGui::BeginChild("##Visualization", ImVec2(0.0f, 0.0f), ImGuiChildFlags_ResizeY | 0);
    ImGui::EndChild();

    ImGui::PopStyleColor();
}

inline void draw_editor_panel(AppState* appstate){
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(10, 10, 10, 255));

    ImGui::BeginChild("##EditorPanel", ImVec2(0.0f, 0.0f), 0);
    ImGui::EndChild();

    ImGui::PopStyleColor();
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
        SDL_GetWindowSizeInPixels(appstate->window, &appstate->ui_data.window_width, &appstate->ui_data.window_hight);
        ImGui::NewFrame();
        

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));

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