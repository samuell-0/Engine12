add_subdirectory(sdl3)
add_subdirectory(glm)
add_subdirectory(vk-bootstrap)

# Vulkan-Headers
add_library(VulkanHeaders INTERFACE)
target_include_directories(VulkanHeaders INTERFACE ${CMAKE_SOURCE_DIR}/third_party/vulkan-headers/include)

# VMA (header-only)
add_library(vma INTERFACE)
target_include_directories(vma INTERFACE ${CMAKE_SOURCE_DIR}/third_party/vma/include)

# ImGui
add_library(imgui
    ${CMAKE_SOURCE_DIR}/third_party/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_vulkan.cpp
    ${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_sdl3.cpp
)
target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/third_party/imgui)
target_link_libraries(imgui PUBLIC SDL3::SDL3 VulkanHeaders vulkan)

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_SOURCE_DIR}/third_party/stb)