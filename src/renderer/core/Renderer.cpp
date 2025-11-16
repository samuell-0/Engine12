#include "renderer/core/Renderer.h"
#include "core/Log.hpp"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_sdl3.h"
VkResult Renderer::create_render_pass(AppState* appstate){
    VkAttachmentDescription color_attachment = {};
    color_attachment.format         = appstate->swapchain.image_format;
    color_attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref = {};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass    = {};
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &color_attachment_ref;

    VkSubpassDependency dependency  = {};
    dependency.srcSubpass           = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass           = 0;
    dependency.srcStageMask         = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask        = 0;
    dependency.dstStageMask         = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask        = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo render_pass_info = {};
    render_pass_info.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount        = 1;
    render_pass_info.pAttachments           = &color_attachment;
    render_pass_info.subpassCount           = 1;
    render_pass_info.pSubpasses             = &subpass;
    render_pass_info.dependencyCount        = 1;
    render_pass_info.pDependencies          = &dependency;

    VkResult res = appstate->disp.createRenderPass(&render_pass_info, nullptr, &appstate->render_data.render_pass);
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Error, "unbl to crt renderpass", res);
    return VK_SUCCESS;
}
// inline bool Renderer::draw_frame(AppState* AppState){}

VkResult Renderer::create_sync_objects(AppState* appstate){
    appstate->render_data.available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    appstate->render_data.finished_semaphore.resize(appstate->swapchain.image_count);
    appstate->render_data.in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);
    appstate->render_data.image_in_flight.resize(appstate->swapchain.image_count, VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphore_info = {};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info = {};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < appstate->swapchain.image_count; i++){
        VkResult res = appstate->disp.createSemaphore(&semaphore_info, nullptr, &appstate->render_data.finished_semaphore[i]);
        if (res != VK_SUCCESS)
            return Log::push(LogLevel::Error, "unbl to crt semaphore (in a for loop)", res);
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (appstate->disp.createSemaphore(&semaphore_info, nullptr, &appstate->render_data.available_semaphores[i]) != VK_SUCCESS ||
            appstate->disp.createFence(&fence_info, nullptr, &appstate->render_data.in_flight_fences[i]) != VK_SUCCESS)
            return Log::push(LogLevel::Error, "unbl to crt semaphore or fence (create_sync_objects)", VK_ERROR_INITIALIZATION_FAILED);
    }
    return VK_SUCCESS;
}

void Renderer::clean_up(AppState* appstate){

    // Free command buffers allocated from our command pool before destroying it
    if (!appstate->render_data.command_buffers.empty())
        appstate->disp.freeCommandBuffers( appstate->render_data.command_pool,
                                           static_cast<uint32_t>(appstate->render_data.command_buffers.size()),
                                           appstate->render_data.command_buffers.data());

    for (size_t i = 0; i < appstate->swapchain.image_count; i++) {
        appstate->disp.destroySemaphore(appstate->render_data.finished_semaphore[i], nullptr);
    }
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        appstate->disp.destroySemaphore(appstate->render_data.available_semaphores[i], nullptr);
        appstate->disp.destroyFence(appstate->render_data.in_flight_fences[i], nullptr);
    }

    // Destroy the main command pool (must be after freeing its command buffers)
    appstate->disp.destroyCommandPool(appstate->render_data.command_pool, nullptr);

    // Destroy framebuffers, pipelines and render pass
    for (auto framebuffer : appstate->render_data.framebuffers) {
        appstate->disp.destroyFramebuffer(framebuffer, nullptr);
    }

    appstate->disp.destroyPipeline(appstate->render_data.graphics_pipeline, nullptr);
    appstate->disp.destroyPipelineLayout(appstate->render_data.pipeline_layout, nullptr);
    appstate->disp.destroyRenderPass(appstate->render_data.render_pass, nullptr);

    // Destroy ImGui descriptor pool
    if (appstate->imgui_desc_pool != VK_NULL_HANDLE) {
        appstate->disp.destroyDescriptorPool(appstate->imgui_desc_pool, nullptr);
        appstate->imgui_desc_pool = VK_NULL_HANDLE;
    }

    appstate->swapchain.destroy_image_views(appstate->render_data.swapchain_image_views);

    vkb::destroy_swapchain(appstate->swapchain);
    vkb::destroy_device(appstate->device);
    vkb::destroy_surface(appstate->instance, appstate->surface);
    vkb::destroy_instance(appstate->instance);
}