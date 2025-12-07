#include <imgui_internal.h>
#include "ui/Utils.h"

bool Injected::SetWindowHightByName(const char* name, float height){
    // imguiWindow* window = ImGui::FindWindowByName(name);
    // IM_ASSERT(window != nullptr);
    // window->SizeFull = ImVec2(window->SizeFull.x, height);
    // ImGui::SetWindowSize(name, ImVec2(0, height));
    return true;
}
// bool Injected::SetWindowHightByName(const char* name, float height)
// {
//     imguiContext& g = *Gimgui;
//     imguiWindow* window = ImGui::FindWindowByName(name);

//     // Debug: list all existing windows
//     if (!window) {
//         ImGui::DebugLog(">>> SetWindowHightByName(\"%s\") failed! Existing windows:\n", name);
//         for (imguiWindow* w : g.Windows) {
//             ImGui::DebugLog("  - \"%s\"\n", w->Name);
//         }
//     }

//     IM_ASSERT(window != nullptr && "Window not found! Check name and call order.");
    
//     window->SizeFull = ImVec2(window->SizeFull.x, height);
//     return true;
// }