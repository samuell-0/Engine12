cmake_minimum_required(VERSION 3.14)
project(Engine12 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(GLSLC_EXECUTABLE glslc REQUIRED)
if(NOT GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc not found. Install with 'sudo apt install glslc'.")
endif()

set(SDL_X11_XCURSOR OFF CACHE BOOL "" FORCE)
set(SDL_X11_XSCRNSAVER OFF CACHE BOOL "" FORCE)
set(SDL_X11_XTEST OFF CACHE BOOL "" FORCE)

set(SDL_OPENGL OFF CACHE BOOL "" FORCE)
set(SDL_OPENGLES OFF CACHE BOOL "" FORCE)
set(SDL_EGL OFF CACHE BOOL "" FORCE)
set(SDL_VULKAN ON CACHE BOOL "" FORCE)

set(SDL_AUDIO OFF CACHE BOOL "" FORCE)

set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_RENDER OFF CACHE BOOL "" FORCE)

add_subdirectory(third_party)

add_library(renderer
    src/renderer/core/Device.cpp
    src/renderer/core/Swapchain.cpp
    src/renderer/core/CommandPool.cpp
    src/renderer/core/Renderer.cpp
    src/renderer/resources/Buffer.cpp
    src/renderer/resources/Texture.cpp
    src/renderer/resources/DescriptorManager.cpp
    src/renderer/resources/Sampler.cpp
    src/renderer/pipelines/Pipeline.cpp
    src/renderer/pipelines/ForwardPipeline.cpp
    src/renderer/pipelines/ShadowPipeline.cpp
    src/renderer/pipelines/ComputePipeline.cpp
    src/renderer/pipelines/SkinningPipeline.cpp
    src/renderer/utils/FrameGraph.cpp
    src/renderer/utils/BarrierHelper.cpp
    src/renderer/utils/ShaderModule.cpp
    src/physics/TransformUpdater.cpp
    src/physics/DrawCommandGenerator.cpp
    src/physics/PhysicsThread.cpp
    src/shadertoy/ShaderToyRunner.cpp
    src/shadertoy/EffectPipeline.cpp
    src/ui/UI.cpp
    src/ui/UIController.cpp
    src/ui/Utils.cpp
    src/ui/Animation.cpp
    src/debuging/Log.cpp
    third_party/imgui/imgui_demo.cpp
    
)
target_include_directories(renderer PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(renderer PUBLIC SDL3::SDL3 glm::glm VulkanHeaders vma vk-bootstrap imgui stb vulkan)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE renderer)
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin"
    # RUNTIME_OUTPUT_DIRECTORY_RELEASE "/home/Aero/Documents/projects/test/release"
)
find_package(GTest REQUIRED)
add_executable(renderer_tests
    tests/renderer_tests/TestMain.cpp
    # tests/renderer_tests/DeviceTest.cpp
    # tests/renderer_tests/SwapchainTest.cpp
    # tests/renderer_tests/CommandPoolTest.cpp
    # tests/renderer_tests/BufferTest.cpp
    # tests/renderer_tests/TextureTest.cpp
    # tests/renderer_tests/DescriptorManagerTest.cpp
    # tests/renderer_tests/SamplerTest.cpp
    # tests/renderer_tests/PipelineTest.cpp
    # tests/renderer_tests/ForwardPipelineTest.cpp
    # tests/renderer_tests/ShadowPipelineTest.cpp
    # tests/renderer_tests/ComputePipelineTest.cpp
    # tests/renderer_tests/SkinningPipelineTest.cpp
    # tests/renderer_tests/RendererTest.cpp
    # tests/renderer_tests/FrameGraphTest.cpp
    # tests/renderer_tests/BarrierHelperTest.cpp
    # tests/renderer_tests/ShaderModuleTest.cpp
    # tests/renderer_tests/TransformUpdaterTest.cpp
    # tests/renderer_tests/DrawCommandGeneratorTest.cpp
    # tests/renderer_tests/ShaderToyRunnerTest.cpp
    # tests/renderer_tests/EffectPipelineTest.cpp
)
target_link_libraries(renderer_tests PRIVATE renderer GTest::gtest GTest::gtest_main)
set_target_properties(renderer_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin"
    # RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
)
enable_testing()
add_test(NAME RendererTests COMMAND renderer_tests)

file(GLOB_RECURSE SHADER_FILES ${CMAKE_SOURCE_DIR}/shaders/glsl/*.glsl)
foreach(SHADER ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    set(SPIRV_OUTPUT ${CMAKE_SOURCE_DIR}/shaders/compiled/${SHADER_NAME}.spv)
    add_custom_command(
        OUTPUT ${SPIRV_OUTPUT}
        COMMAND ${GLSLC_EXECUTABLE} ${SHADER} -o ${SPIRV_OUTPUT}
        DEPENDS ${SHADER}
        COMMENT "Compiling shader ${SHADER_NAME}"
    )
    list(APPEND SPIRV_OUTPUTS ${SPIRV_OUTPUT})
endforeach()
add_custom_target(shaders ALL DEPENDS ${SPIRV_OUTPUTS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)