#include "renderer/core/Renderer.h"

bool Renderer::create_render_pass(AppState* appstate){
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

    if (appstate->disp.createRenderPass(&render_pass_info, nullptr, &appstate->render_data.render_pass) != VK_SUCCESS)
        return false;
    return true;
}
// inline bool Renderer::draw_frame(AppState* AppState){}

bool Renderer::create_sync_objects(AppState* appstate){
    appstate->render_data.available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    appstate->render_data.finished_semaphore.resize(appstate->swapchain.image_count);
    appstate->render_data.in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);
    appstate->render_data.image_in_flight.resize(appstate->swapchain.image_count, VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphore_info = {};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info = {};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < appstate->swapchain.image_count; i++)
        if (appstate->disp.createSemaphore(&semaphore_info, nullptr, &appstate->render_data.finished_semaphore[i]) != VK_SUCCESS)
            return false;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (appstate->disp.createSemaphore(&semaphore_info, nullptr, &appstate->render_data.available_semaphores[i]) != VK_SUCCESS ||
            appstate->disp.createFence(&fence_info, nullptr, &appstate->render_data.in_flight_fences[i]) != VK_SUCCESS)
            return false;
    }
    return true;
}