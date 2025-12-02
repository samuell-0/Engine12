#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_vulkan.h>

#include "core/State.hpp"
#include "ui/IconsAwesome6.h"

#include "core/Log.hpp"
inline void col_resize_callback(float new_width, uint8_t col_index){
    ImGui::SetColumnWidth(col_index, new_width);
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

inline void draw_window_CTRL(AppState* appstate){
    float window_width  = appstate->ui_data.window_width;
    float start  = (window_width / 2) - 75.0f;
    // ImGui::GetWindowDrawList()->AddRect(ImVec2(start, 20.0f), ImVec2(start + 150.0f, 45.0f), IM_COL32(255, 255, 255, 255));
    if ((!ImGui::IsMouseDragging(ImGuiMouseButton_Left)) && ImGui::IsMouseHoveringRect(ImVec2(start - 4, 16.0f), ImVec2(start + 154.0f, 49.0f))){
        ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(20, 20, 20, 250));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 40.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 7.0f);

        ImGui::OpenPopup("##WindowCTRLPopup");
        ImGui::SetNextWindowPos(ImVec2(start, 20.0f));
        if (ImGui::BeginPopup("##WindowCTRLPopup")){

            float spacing = 37.5f;
            // NOTE: static assuming there is no other way in the app to change it
            static bool fullscreen = (bool)(SDL_GetWindowFlags(appstate->window) & SDL_WINDOW_FULLSCREEN);

            if (ImGui::Button(ICON_FA_CIRCLE "##iu", ImVec2(25.0f, 25.0f)))
                0;

            ImGui::SameLine(0.0f, spacing);
            if (fullscreen){
                if (ImGui::Button(ICON_FA_COMPRESS "##compress", ImVec2(25.0f, 25.0f))){
                    if (!SDL_SetWindowFullscreen(appstate->window, false)){
                        Log::push(LogLevel::Warning, "could not turn off fullscreen: " + std::string(SDL_GetError()));
                    }
                    else{
                        fullscreen = false;
                        ImGuiContext& g = *ImGui::GetCurrentContext();
                        
                    }
                }
            }
            else{
                if (ImGui::Button(ICON_FA_FULLSCREEN "##fullscreen", ImVec2(25.0f, 25.0f))){
                    if (!SDL_SetWindowFullscreen(appstate->window, true)){
                        Log::push(LogLevel::Warning, "could not turn on fullscreen: " + std::string(SDL_GetError()));
                    }
                    else{
                        fullscreen = true;
                    }
                }
            }

            ImGui::SameLine(0.0f, spacing);
            if (ImGui::Button(ICON_FA_CLOSE "##close", ImVec2(25.0f, 25.0f))){
                SDL_Event quit_event;
                quit_event.type = SDL_EVENT_QUIT;
                SDL_PushEvent(&quit_event);
            }
            ImGui::EndPopup();
        }
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor();
    }
}

inline void draw_test_setting_window(){
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 100.f);
    ImGui::OpenPopup("##TestSettingPopup");
    ImGui::SetNextWindowSize(ImVec2(100, 100));
    if (ImGui::BeginPopup("##TestSettingPopup")){
        ImGui::Text("iybsna");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
}

inline void draw_left_panel(AppState* appstate){
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(10, 10, 10, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(5, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_TabMinWidthBase, ImGui::GetWindowWidth() / 2);
    float col_width = ImGui::GetColumnWidth();
    bool show_col_content = (col_width >= 100);

    ImGui::BeginChild("##LeftPanel", ImVec2(0.0f, appstate->ui_data.window_hight));
    if (ImGui::BeginTabBar("##TabBar")){
        bool api_tab_is_open = ImGui::BeginTabItem("Vulkan");
        if (ImGui::IsItemHovered()){
            ImGui::BeginTooltip();
            ImGui::Text("API specific details");
            ImGui::EndTooltip();
        }
        if (api_tab_is_open){
            if (show_col_content){
                // if (ImGui::BeginMenu(ICON_FA_SAVE " kj8j")){
                    ImGui::MenuItem(ICON_FA_FILE " New");
                    ImGui::MenuItem(ICON_FA_FOLDER " Open");
                    ImGui::MenuItem(ICON_FA_SAVE " Save");
                //     ImGui::EndMenu();
                // }       
                // if (ImGui::Button(ICON_FA_TSET, ImVec2(ImGui::GetWindowWidth(), 0.0f)))
                // {
                //     ImGui::OpenPopup("##TestSettingPopup");
                //     ImGui::SetNextWindowSize(ImVec2(100, 100));
                //     if (ImGui::BeginPopup("##TestSettingPopup")){
                //         ImGui::Text("iybsna");
                //         ImGui::EndPopup();
                //     }          
                // }
                    // if (appstate->ui_data.which_setting != SettingOpened::Test)
                    //     appstate->ui_data.which_setting = SettingOpened::Test;
                    // else
                    //     appstate->ui_data.which_setting = SettingOpened::None;
                // ImGui::ShowMetricsWindow();
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
            if (show_col_content){
            }
            
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);    
}

inline void draw_view_window(AppState* appstate){
    ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, 0.0f), ImVec2(appstate->ui_data.window_width, appstate->ui_data.window_hight - 25.0f));
    ImGui::BeginChild("##ViewWindow", ImVec2(0.0f, 0.0f), ImGuiChildFlags_ResizeY, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
    // switch (appstate->ui_data.which_setting)
    // {
    //     case SettingOpened::Test:
    //         draw_test_setting_window(appstate);
    //     default:
    //         break;
    // }
    ImGui::EndChild();
}

inline void draw_buttom_panel(AppState* appstate){
    if (ImGui::GetContentRegionAvail().y <= 100)
        return;
    if (ImGui::BeginTable("##BottomTable", 2, ImGuiTableFlags_Resizable)){

        ImGui::TableNextRow();
        
        ImGui::TableNextColumn();

        ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(16, 28, 33, 255));
        ImGui::BeginChild("##LeftColumn", ImVec2(0.0f, 0.0f));
            ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::TableNextColumn();

        ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(10, 10, 10, 255));
        ImGui::BeginChild("##RightColumn", ImVec2(0.0f, 0.0f));
            ImGui::EndChild();

        ImGui::PopStyleColor();

        ImGui::EndTable();
    }
}

inline void draw_visualization_panel(AppState* appstate){
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(16, 28, 33, 255));
    float col_width = ImGui::GetColumnWidth();
    bool show_col_content = (col_width >= 100);
    ImGui::BeginChild("##Visualization", ImVec2(0.0f, 0.0f), ImGuiChildFlags_ResizeY);

    ImGui::EndChild();

    ImGui::PopStyleColor();
}

inline void draw_editor_panel(AppState* appstate){
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(10, 10, 10, 255));

    ImGui::BeginChild("##EditorPanel", ImVec2(0.0f, 0.0f), 0);
    ImGui::EndChild();

    ImGui::PopStyleColor();
}

inline void draw_middle_panel(AppState* appstate){
    ImGui::BeginChild("##MiddlePanel", ImVec2(0.0f, 0.0f), 0, ImGuiWindowFlags_MenuBar);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 0.0f));
    if (ImGui::BeginMenuBar()){
        if (ImGui::BeginMenu(ICON_FA_SAVE " kjj")){
            ImGui::MenuItem(ICON_FA_FILE " New"); ImGui::Separator();
            ImGui::MenuItem(ICON_FA_FOLDER " Open"); ImGui::Separator();
            ImGui::MenuItem(ICON_FA_SAVE " Save"); ImGui::Separator();
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
    draw_view_window(appstate);
    ImGui::Separator();
    draw_buttom_panel(appstate);
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
        
        draw_window_CTRL(appstate);

        if (ImGui::BeginTable("##MainLayout", 3, ImGuiTableFlags_Resizable)){
            ImGui::TableNextRow();

            if (ImGui::TableNextColumn()){
                float col_width = ImGui::GetColumnWidth();
                bool show_col_content = (col_width >= 100);
                if (show_col_content){
                    draw_left_panel(appstate);
                }
            }
            if (ImGui::TableNextColumn()){
                float col_width = ImGui::GetColumnWidth();
                bool show_col_content = (col_width >= 100);
                if (show_col_content){
                    draw_middle_panel(appstate);
                }
                else{// No simulation to display(headless?)
                }
            }
            if (ImGui::TableNextColumn()){
                float col_width = ImGui::GetColumnWidth();
                bool show_col_content = (col_width >= 100);
                if (show_col_content){
                    draw_secondary_menu_bar(appstate);
                    draw_visualization_panel(appstate);
                    draw_editor_panel(appstate);
                }
            }
            ImGui::EndTable();
        }
        ImGui::End();

        ImGui::Render();
    }

}