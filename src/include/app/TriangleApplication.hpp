#pragma once

#include <common/header.hpp>
#include <vulkan/instance/instance.hpp>
#include <vulkan/validation_layer/validation.hpp>
#include <vulkan/surface/surface.hpp>
#include <vulkan/device/device.hpp>
#include <vulkan/swapchain/swapchain.hpp>
#include <vulkan/renderpass/renderpass.hpp>
#include <vulkan/graphicspipeline/graphicspipeline.hpp>

class TriangleApplication
{
    public:
        void run();

    private:
        void initialize_window();
        void initialize_vulkan();
        void main_loop();
        void cleanup();

    private:
        GLFWwindow* m_window = nullptr;
        Instance* m_instance = nullptr;
        Validation* m_validation = nullptr;
        Surface* m_surface = nullptr;
        Device* m_device = nullptr;
        SwapChain* m_swapchain = nullptr;
        RenderPass* m_renderpass = nullptr;
        GraphicsPipeline* m_graphicspipeline = nullptr;
};