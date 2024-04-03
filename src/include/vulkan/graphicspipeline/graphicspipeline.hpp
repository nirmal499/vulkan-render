#pragma once

#include <common/header.hpp>
#include <vulkan/device/device.hpp>
#include <vulkan/swapchain/swapchain.hpp>

class GraphicsPipeline
{
    public:
        const VkPipelineLayout& get_pipeline_layout();
        const VkPipeline& get_graphics_pipeline();
        const VkRenderPass& get_render_pass();

        void graphicspipeline_initialization(Device* device, SwapChain* swapchain);
        void create_graphics_pipeline();
        void create_render_pass();

        void destroy();
    private:
        VkShaderModule createShaderModule(const std::vector<char>& code);

    private:
        VkPipeline m_graphics_pipeline = VK_NULL_HANDLE;
        VkPipelineLayout m_pipeline_layout = VK_NULL_HANDLE;
        VkRenderPass m_render_pass = VK_NULL_HANDLE;

        /* helper objects */
        Device* m_temp_device;
        SwapChain* m_temp_swapchain;
};