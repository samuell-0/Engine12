#include "renderer/core/Swapchain.h"
#include "renderer/core/CommandPool.h"
#include "core/Log.hpp"
VkResult create_msaa_color_image(AppState* appstate){
    VkImageCreateInfo image_info{};
    image_info.sType    = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.format   = appstate->swapchain.image_format;
    image_info.usage    = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    image_info.extent = { appstate->swapchain.extent.width,
                          appstate->swapchain.extent.height, 1 };
    image_info.mipLevels        = 1;
    image_info.arrayLayers      = 1;
    image_info.imageType        = VK_IMAGE_TYPE_2D;
    // image_info.flags
    image_info.samples          = VK_SAMPLE_COUNT_4_BIT;

    if (appstate->disp.createImage(&image_info, nullptr, &appstate->render_data.msaa_image) != VK_SUCCESS)
        return Log::push(LogLevel::Info, "faild to create msaa image...continuing", VK_ERROR_INITIALIZATION_FAILED);

    VkMemoryRequirements mem_req;
    appstate->disp.getImageMemoryRequirements(appstate->render_data.msaa_image, &mem_req);

    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_req.size;
    alloc_info.memoryTypeIndex ;//NOTE: I thing I must assignt this....

    
    appstate->disp.allocateMemory(&alloc_info, nullptr, &appstate->render_data.msaa_memory);
    
    appstate->disp.bindImageMemory(appstate->render_data.msaa_image, appstate->render_data.msaa_memory, 0);


    VkImageViewCreateInfo view_info{};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = appstate->render_data.msaa_image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = appstate->swapchain.image_format;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    if (appstate->disp.createImageView(&view_info, nullptr, &appstate->render_data.msaa_image_view) != VK_SUCCESS)
        return Log::push(LogLevel::Info, "faild to create msaa image view...continuing", VK_ERROR_INITIALIZATION_FAILED);

    return VK_SUCCESS;
}
VkResult Swapchain::create_framebuffers(AppState* appstate) {
    create_msaa_color_image(appstate);
    appstate->render_data.swapchain_image_views = appstate->swapchain.get_image_views().value();

    appstate->render_data.framebuffers.resize(appstate->render_data.swapchain_image_views.size());

    for (size_t i = 0; i < appstate->render_data.swapchain_image_views.size(); i++) {
        VkImageView attachments[] = { appstate->render_data.msaa_image_view, appstate->render_data.swapchain_image_views[i] };

        VkFramebufferCreateInfo framebuffer_info{};
        framebuffer_info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass      = appstate->render_data.render_pass;
        framebuffer_info.attachmentCount = 2;
        framebuffer_info.pAttachments    = attachments;
        framebuffer_info.width           = appstate->swapchain.extent.width;
        framebuffer_info.height          = appstate->swapchain.extent.height;
        framebuffer_info.layers          = 1;

        VkResult res = appstate->disp.createFramebuffer(&framebuffer_info, nullptr, &appstate->render_data.framebuffers[i]);
        if (res != VK_SUCCESS)
            return Log::push(LogLevel::Error, "unbl to crt frame bffr", VK_ERROR_INITIALIZATION_FAILED);
    }
    return VK_SUCCESS;
}
// VkSurfaceFormatKHR choose_swapchain_format(AppState* appstate) {
//     uint32_t count = 0;
//     vkGetPhysicalDeviceSurfaceFormatsKHR(appstate->device.physical_device.physical_device, appstate->surface, &count, nullptr);
//     if (count == 0){
//         Log::push(LogLevel::Warning, "Failed to get avalible formats....falling back");
//         return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
//     }
//     std::vector<VkSurfaceFormatKHR> formats(count);
//     vkGetPhysicalDeviceSurfaceFormatsKHR(appstate->device.physical_device.physical_device, appstate->surface, &count, formats.data());
//     for (const VkSurfaceFormatKHR& fmt : formats) {
//         if (fmt.format == VK_FORMAT_B8G8R8A8_SRGB && fmt.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)//I did this for imgui if you want to remember...
//             return fmt;
//     }
//     Log::push(LogLevel::Warning, "None of the desired formats are supported by the physical device....falling back");
//     return formats[0];
// }
VkResult Swapchain::create_swapchain(AppState* appstate){
    VkSurfaceFormatKHR desired_format = {};
    desired_format.format = VK_FORMAT_B8G8R8A8_UNORM;//why did I use bgr instead of rbg?   I did this for imgui if you want to remember...I made sure it is suppotted on my current pc if you wondering(choose_swapchain_format)
    desired_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; // Correct color space

    vkb::SwapchainBuilder       swapchain_builder{ appstate->device };
    vkb::Result<vkb::Swapchain> swap_result = swapchain_builder
                                                .set_old_swapchain(appstate->swapchain)
                                                .set_desired_format(desired_format)
                                                .build();
    if (!swap_result.has_value())
        return Log::push(LogLevel::Error, "failed to create swachain...", swap_result.vk_result());
    
    // swap_result.value().image_format = VK_FORMAT_R8G8B8A8_UNORM;//maybe I need to expose this for latter?

    vkb::destroy_swapchain(appstate->swapchain);
    appstate->swapchain = swap_result.value();

    return VK_SUCCESS;
}
VkResult Swapchain::recreate_swapchain(AppState* appstate){
    appstate->disp.deviceWaitIdle();

    appstate->disp.destroyCommandPool(appstate->render_data.command_pool, nullptr);

    for (VkFramebuffer framebuffer : appstate->render_data.framebuffers) {
        appstate->disp.destroyFramebuffer(framebuffer, nullptr);
    }

    appstate->swapchain.destroy_image_views(appstate->render_data.swapchain_image_views);

    if (create_swapchain(appstate)                     != VK_SUCCESS)  return Log::push(LogLevel::Error, "unbl to crt swp chain(re)",  VK_ERROR_INITIALIZATION_FAILED);;
    if (create_framebuffers(appstate)                  != VK_SUCCESS)  return Log::push(LogLevel::Error, "unbl to crt frame bffr(re)", VK_ERROR_INITIALIZATION_FAILED);;
    if (CommandPool::create_command_pool(appstate)     != VK_SUCCESS)  return Log::push(LogLevel::Error, "unbl to crt cmd pool(re)",   VK_ERROR_INITIALIZATION_FAILED);;
    if (CommandPool::allocate_command_buffers(appstate)!= VK_SUCCESS)  return Log::push(LogLevel::Error, "unbl to crt cmd bffr(re)",   VK_ERROR_INITIALIZATION_FAILED);;

    return VK_SUCCESS;
}