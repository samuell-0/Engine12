#include "renderer/core/CommandPool.h"
#include <iostream>
#include "core/Log.hpp"

VkResult CommandPool::create_command_pool(AppState* appstate){
    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = appstate->device.get_queue_index(vkb::QueueType::graphics).value();

    VkResult res = appstate->disp.createCommandPool(&pool_info, nullptr, &appstate->render_data.command_pool);
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Error, "unbl to crt cmd pool", res);
    return VK_SUCCESS;
}
VkResult CommandPool::create_command_buffers(AppState* appstate){

    appstate->render_data.command_buffers.resize(appstate->render_data.framebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = appstate->render_data.command_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)appstate->render_data.command_buffers.size();

    VkResult res = appstate->disp.allocateCommandBuffers(&allocInfo, appstate->render_data.command_buffers.data());
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Error, "unbl to alloc cmd bffr", res);

    for (size_t i = 0; i < appstate->render_data.command_buffers.size(); i++) {
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        // begin_info.flags = VK_COMMAND_BUFFER_U

        VkResult res = appstate->disp.beginCommandBuffer(appstate->render_data.command_buffers[i], &begin_info);
        if (res != VK_SUCCESS)
            return Log::push(LogLevel::Error, "unbl to begin cmd bffr", res);

        VkRenderPassBeginInfo render_pass_info = {};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = appstate->render_data.render_pass;
        render_pass_info.framebuffer = appstate->render_data.framebuffers[i];
        render_pass_info.renderArea.offset = { 0, 0 };
        render_pass_info.renderArea.extent = appstate->swapchain.extent;
        VkClearValue clearColor{ { { 0.0f, 0.0f, 0.0f, 1.0f } } };
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clearColor;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)appstate->swapchain.extent.width;
        viewport.height = (float)appstate->swapchain.extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = appstate->swapchain.extent;

        appstate->disp.cmdBeginRenderPass(appstate->render_data.command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
        
        appstate->disp.cmdSetViewport(appstate->render_data.command_buffers[i], 0, 1, &viewport);
        appstate->disp.cmdSetScissor(appstate->render_data.command_buffers[i], 0, 1, &scissor);

        appstate->disp.cmdBindPipeline(appstate->render_data.command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, appstate->render_data.graphics_pipeline);

        appstate->disp.cmdDraw(appstate->render_data.command_buffers[i], 3, 1, 0, 0);

        appstate->disp.cmdEndRenderPass(appstate->render_data.command_buffers[i]);

        res = appstate->disp.endCommandBuffer(appstate->render_data.command_buffers[i]);
        if (res != VK_SUCCESS)
            return Log::push(LogLevel::Warning, "unbl to end cmd buffer");
    }
    
    return VK_SUCCESS;
}
VkCommandBuffer CommandPool::begin_single_time_command(AppState* appstate){
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool        = appstate->render_data.command_pool;
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
VkResult CommandPool::end_single_time_command(AppState* appstate, VkCommandBuffer commandBuffer){
    VkResult res = appstate->disp.endCommandBuffer(commandBuffer);
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Error, "unbl to end cmd buffer(end_single_time_command)");
    
    VkSubmitInfo submitInfo = {};
    submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &commandBuffer;

    res = appstate->disp.queueSubmit(appstate->render_data.graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Error, "unbl to sbmt(end_single_time_command)");
    
    res = appstate->disp.queueWaitIdle(appstate->render_data.graphics_queue);  // Wait for completion
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Error, "at end_single_time_command");
    
    appstate->disp.freeCommandBuffers(appstate->render_data.command_pool, 1, &commandBuffer);
    
    return VK_SUCCESS;
}