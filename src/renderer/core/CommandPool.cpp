#include "renderer/core/CommandPool.h"
#include <iostream>
#include "core/Log.hpp"
#include <imgui.h>
#include "backends/imgui_impl_vulkan.h"

VkResult CommandPool::create_command_pool(AppState *appstate)
{
    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    // Allow resetting/re-recording command buffers individually. This is
    // required because we re-record per-image primary command buffers each
    // frame (see CommandPool::record_command_buffer). Without this flag,
    // calling vkBeginCommandBuffer on a previously recorded command buffer
    // is invalid and triggers VUID-vkBeginCommandBuffer-commandBuffer-00050.
    // using this flag has more overhead(FIXME)
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = appstate->device.get_queue_index(vkb::QueueType::graphics).value();

    VkResult res = appstate->disp.createCommandPool(&pool_info, nullptr, &appstate->render_data.command_pool);
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Error, "unbl to crt cmd pool", res);
    return VK_SUCCESS;
}

VkResult CommandPool::allocate_command_buffers(AppState *appstate)
{
    appstate->render_data.command_buffers.resize(appstate->render_data.framebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = appstate->render_data.command_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)appstate->render_data.command_buffers.size();

    VkResult res = appstate->disp.allocateCommandBuffers(&allocInfo, appstate->render_data.command_buffers.data());
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Error, "unbl to alloc cmd bffr", res);
    // Note: we allocate command buffers here. Recording will be done per-frame
    // via CommandPool::record_command_buffer so dynamic data like ImGui draw
    // lists are recorded fresh every frame.

    return VK_SUCCESS;
}

VkResult CommandPool::record_command_buffer(AppState* appstate, uint32_t i)
{
    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VkResult res = appstate->disp.beginCommandBuffer(appstate->render_data.command_buffers[i], &begin_info);
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Error, "unbl to begin cmd bffr", res);

    VkRenderPassBeginInfo render_pass_info = {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = appstate->render_data.render_pass;
    render_pass_info.framebuffer = appstate->render_data.framebuffers[i];
    render_pass_info.renderArea.offset = {0, 0};
    render_pass_info.renderArea.extent = appstate->swapchain.extent;
    render_pass_info.clearValueCount = 1;
    render_pass_info.pClearValues = &appstate->clearColor;

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)appstate->swapchain.extent.width;
    viewport.height = (float)appstate->swapchain.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = appstate->swapchain.extent;

    appstate->disp.cmdBeginRenderPass(appstate->render_data.command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

    appstate->disp.cmdSetViewport(appstate->render_data.command_buffers[i], 0, 1, &viewport);
    appstate->disp.cmdSetScissor(appstate->render_data.command_buffers[i], 0, 1, &scissor);

    appstate->disp.cmdBindPipeline(appstate->render_data.command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, appstate->render_data.graphics_pipeline);

    // draw application geometry (triangle)
    appstate->disp.cmdDraw(appstate->render_data.command_buffers[i], 3, 1, 0, 0);

    // Render Dear ImGui draw data into the same command buffer (inside the render pass)
    ImDrawData *imgui_draw_data = ImGui::GetDrawData();
    if (imgui_draw_data == nullptr)
        return Log::push(LogLevel::Warning, "imgui draw data null ptr");
    // Let the ImGui backend use its own pipeline (created during Init)
    // Passing the app's graphics pipeline here causes ImGui to render with
    // the wrong pipeline state (depth, blending, vertex layout) and can
    // occlude or break the triangle rendering.
    // 
    ImGui_ImplVulkan_RenderDrawData(imgui_draw_data, appstate->render_data.command_buffers[i]);

    appstate->disp.cmdEndRenderPass(appstate->render_data.command_buffers[i]);

    res = appstate->disp.endCommandBuffer(appstate->render_data.command_buffers[i]);
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Warning, "unbl to end cmd buffer");

    return VK_SUCCESS;
}

VkCommandBuffer CommandPool::begin_single_time_command(AppState *appstate)
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = appstate->render_data.command_pool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer cmd;
    VkResult res = appstate->disp.allocateCommandBuffers(&allocInfo, &cmd);
    if (res != VK_SUCCESS)
        Log::push(LogLevel::Warning, "unbl to alloc cmd buffer(begin_single_time_command)");
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    res = appstate->disp.beginCommandBuffer(cmd, &beginInfo);
    if (res != VK_SUCCESS)
        Log::push(LogLevel::Warning, "unbl to bgn cmd buffer(begin_single_time_command)");
    return cmd;
}

VkResult CommandPool::end_single_time_command(AppState *appstate, VkCommandBuffer commandBuffer)
{
    VkResult res = appstate->disp.endCommandBuffer(commandBuffer);
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Error, "unbl to end cmd buffer(end_single_time_command)");

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    res = appstate->disp.queueSubmit(appstate->render_data.graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Error, "unbl to sbmt(end_single_time_command)");

    res = appstate->disp.queueWaitIdle(appstate->render_data.graphics_queue); // Wait for completion
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Error, "at end_single_time_command");

    appstate->disp.freeCommandBuffers(appstate->render_data.command_pool, 1, &commandBuffer);

    return VK_SUCCESS;
}