#pragma once
#include "core/State.hpp"
#include "renderer/core/Swapchain.h"
#include "renderer/core/CommandPool.h"
#include "core/Log.hpp"
#define MAX_FRAMES_IN_FLIGHT 4
// to be acquired, it must be done being presented and all
// but, the gpu might still be doig staff to it, so let's make sure it's done by being sure the images's last frame is done
// but still, this might not cut it, so we should add semaphores. like wtf bro
namespace Renderer{
    VkResult create_render_pass(AppState* appstate);
    VkResult create_sync_objects(AppState* appstate);
    inline VkResult draw_frame(AppState* appstate){
        appstate->disp.waitForFences(1, &appstate->render_data.in_flight_fences[appstate->render_data.current_frame], VK_TRUE, UINT64_MAX);

        uint32_t image_index = 0;
        //semaphore param synchronizes only with the presentation engine.
        // but since the last frame could be using the image(yes, even after the presentation engine is done with it, although rare), we also check the last frame's fence
        VkResult res = appstate->disp.acquireNextImageKHR(
            appstate->swapchain, UINT64_MAX, appstate->render_data.available_semaphores[appstate->render_data.current_frame], VK_NULL_HANDLE, &image_index);

        if (res == VK_ERROR_OUT_OF_DATE_KHR)
            return Swapchain::recreate_swapchain(appstate);
        else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR)
            return Log::push(LogLevel::Error, "idk(draw_frame)", res);
        // else if (res != VK_SUBOPTIMAL_KHR)
        //     Log::push(LogLevel::Warning, "idk, (draw_frame)");

        if (appstate->render_data.image_in_flight[image_index] != VK_NULL_HANDLE)
            appstate->disp.waitForFences(1, &appstate->render_data.image_in_flight[image_index], VK_TRUE, UINT64_MAX);
        
        appstate->render_data.image_in_flight[image_index] = appstate->render_data.in_flight_fences[appstate->render_data.current_frame];

        // Re-record the primary command buffer for this image so dynamic
        // data (ImGui draw lists) are recorded fresh for the current frame.
        if (CommandPool::record_command_buffer(appstate, image_index) != VK_SUCCESS)
            return Log::push(LogLevel::Error, "failed to record command buffer (draw_frame)", VK_ERROR_UNKNOWN);

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

        res = appstate->disp.queueSubmit(appstate->render_data.graphics_queue, 1, &submitInfo, appstate->render_data.in_flight_fences[appstate->render_data.current_frame]);
        if (res != VK_SUCCESS)
            return Log::push(LogLevel::Error, "queue not being submitted", res);

        VkPresentInfoKHR present_info = {};
        present_info.sType            = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores    = signal_semaphores;

        VkSwapchainKHR swapChains[] = { appstate->swapchain };
        present_info.swapchainCount = 1;
        present_info.pSwapchains    = swapChains;

        present_info.pImageIndices  = &image_index;

        res = appstate->disp.queuePresentKHR(appstate->render_data.present_queue, &present_info);
        if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
            return Swapchain::recreate_swapchain(appstate);

        appstate->render_data.current_frame = (appstate->render_data.current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
        return res;
        }
    void clean_up(AppState* appstate);
    }