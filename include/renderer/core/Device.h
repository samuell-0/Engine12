#pragma once
#include "window/State.hpp"
namespace Device{
    bool device_initialization(AppState* appstate){
        vkb::InstanceBuilder instance_builder;
        vkb::Result<vkb::Instance> result_result = instance_builder.use_default_debug_messenger().request_validation_layers().build();
        if (!result_result) return false;
        appstate->instance  = result_result.value();
        appstate->inst_disp = appstate->instance.make_table();

        SDL_Vulkan_CreateSurface(appstate->window, appstate->instance.instance, nullptr, &appstate->surface);

        vkb::PhysicalDeviceSelector      phys_device_selector(appstate->instance);
        vkb::Result<vkb::PhysicalDevice> phys_device_result = phys_device_selector.set_surface(appstate->surface).select();
        if (!phys_device_result) return false;
        vkb::PhysicalDevice phys_device = phys_device_result.value();

        vkb::DeviceBuilder       device_builder{phys_device};
        vkb::Result<vkb::Device> device_result = device_builder.build();
        if (!device_result) return false;
        appstate->device = device_result.value();
        appstate->disp   = appstate->device.make_table();

        return true;
    }
    bool get_queues(AppState* appstate){// should not be here
        vkb::Result<VkQueue> graphics_queue_result = appstate->device.get_queue(vkb::QueueType::graphics);
        if (!graphics_queue_result) return false;
        appstate->render_data.graphics_queue = graphics_queue_result.value();

        vkb::Result<VkQueue> present_queue_result = appstate->device.get_queue(vkb::QueueType::graphics);
        if (!present_queue_result) return false;
        appstate->render_data.present_queue = present_queue_result.value();
        
        return true;
    }
}