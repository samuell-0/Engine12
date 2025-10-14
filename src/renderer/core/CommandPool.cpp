#include "renderer/core/CommandPool.h"
#include <iostream>
bool CommandPool::create_command_pool(AppState* appstate){
    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = appstate->device.get_queue_index(vkb::QueueType::graphics).value();

    if (appstate->disp.createCommandPool(&pool_info, nullptr, &appstate->render_data.command_pool) != VK_SUCCESS)
        return false;
    return true;
}
bool CommandPool::create_command_buffers(AppState* appstate){
    appstate->render_data.command_buffers.resize(appstate->render_data.framebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = appstate->render_data.command_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)appstate->render_data.command_buffers.size();

    if (appstate->disp.allocateCommandBuffers(&allocInfo, appstate->render_data.command_buffers.data()) != VK_SUCCESS)
        return false;

    for (size_t i = 0; i < appstate->render_data.command_buffers.size(); i++) {
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (appstate->disp.beginCommandBuffer(appstate->render_data.command_buffers[i], &begin_info) != VK_SUCCESS)
            return false;

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

        appstate->disp.cmdSetViewport(appstate->render_data.command_buffers[i], 0, 1, &viewport);
        appstate->disp.cmdSetScissor(appstate->render_data.command_buffers[i], 0, 1, &scissor);

        appstate->disp.cmdBeginRenderPass(appstate->render_data.command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

        appstate->disp.cmdBindPipeline(appstate->render_data.command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, appstate->render_data.graphics_pipeline);

        appstate->disp.cmdDraw(appstate->render_data.command_buffers[i], 3, 1, 0, 0);

        appstate->disp.cmdEndRenderPass(appstate->render_data.command_buffers[i]);
        // {
        // VkImageMemoryBarrier barrier = {};
        // barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        // barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        // barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        // barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        // barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        // barrier.image = appstate->render_data.swapchain_images[i];
        // barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        // barrier.subresourceRange.baseMipLevel = 0;
        // barrier.subresourceRange.levelCount = 1;
        // barrier.subresourceRange.baseArrayLayer = 0;
        // barrier.subresourceRange.layerCount = 1;
        // barrier.srcAccessMask = 0;
        // barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        // appstate->disp.cmdPipelineBarrier(
        //     appstate->render_data.command_buffers[i],
        //     VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        //     VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        //     0,
        //     0, nullptr,
        //     0, nullptr,
        //     1, &barrier
        // );
        // }
        if (appstate->disp.endCommandBuffer(appstate->render_data.command_buffers[i]) != VK_SUCCESS)
            return false;
    }
    return true;
}