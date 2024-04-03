#pragma once

#include <common/header.hpp>

class SwapChain;
class Device;

class RenderPass
{
    public:
        void renderpass_initialization(Device* device, SwapChain* swapchain);
        const VkRenderPass& get_object();
        void create_render_pass();

        void destroy();

    private:
        VkRenderPass m_render_pass = VK_NULL_HANDLE;

        /* helper objects */
        SwapChain* m_temp_swapchain = nullptr;
        Device* m_temp_device = nullptr;

};