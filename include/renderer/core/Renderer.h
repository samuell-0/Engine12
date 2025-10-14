#pragma once
#include "window/State.hpp"
#include "renderer/core/Swapchain.h"
#include <iostream>
#define MAX_FRAMES_IN_FLIGHT 4
// to be acquired, it must be done being presented and all
// but, the gpu might still be doig staff to it, so let's make sure it's done by being sure the images's last frame is done
// but still, this might not cut it, so we should add semaphores. like wtf bro
namespace Renderer{
    bool create_render_pass(AppState* appstate);
    bool create_sync_objects(AppState* appstate);
    inline bool draw_frame(AppState* appstate){
        appstate->disp.waitForFences(1, &appstate->render_data.in_flight_fences[appstate->render_data.current_frame], VK_TRUE, UINT64_MAX);

        uint32_t image_index = 0;
        //semaphore param synchronizes only with the presentation engine.
        // but since the last frame could be using the image(yes, even after the presentation engine is done with it, although rare), we also check the last frame's fence
        VkResult result = appstate->disp.acquireNextImageKHR(
            appstate->swapchain, UINT64_MAX, appstate->render_data.available_semaphores[appstate->render_data.current_frame], VK_NULL_HANDLE, &image_index);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
            return Swapchain::recreate_swapchain(appstate);
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            return false;

        if (appstate->render_data.image_in_flight[image_index] != VK_NULL_HANDLE)
            appstate->disp.waitForFences(1, &appstate->render_data.image_in_flight[image_index], VK_TRUE, UINT64_MAX);
        
        appstate->render_data.image_in_flight[image_index] = appstate->render_data.in_flight_fences[appstate->render_data.current_frame];

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore wait_semaphores[]       = { appstate->render_data.available_semaphores[appstate->render_data.current_frame] };
        VkPipelineStageFlags wait_stages[]  = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount       = 1;
        submitInfo.pWaitSemaphores          = wait_semaphores;
        submitInfo.pWaitDstStageMask        = wait_stages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers    = &appstate->render_data.command_buffers[image_index];

        VkSemaphore signal_semaphores[] = { appstate->render_data.finished_semaphore[image_index] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores    = signal_semaphores;

        appstate->disp.resetFences(1, &appstate->render_data.in_flight_fences[appstate->render_data.current_frame]);

        if (appstate->disp.queueSubmit(appstate->render_data.graphics_queue, 1, &submitInfo, appstate->render_data.in_flight_fences[appstate->render_data.current_frame]) != VK_SUCCESS)
            return false;

        VkPresentInfoKHR present_info = {};
        present_info.sType            = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores    = signal_semaphores;

        VkSwapchainKHR swapChains[] = { appstate->swapchain };
        present_info.swapchainCount = 1;
        present_info.pSwapchains    = swapChains;

        present_info.pImageIndices  = &image_index;

        result = appstate->disp.queuePresentKHR(appstate->render_data.present_queue, &present_info);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
            return Swapchain::recreate_swapchain(appstate);
        else if (result != VK_SUCCESS)
            return false;

        appstate->render_data.current_frame = (appstate->render_data.current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
        return true;
        }
}