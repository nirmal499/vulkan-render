#pragma once

#include <common/header.hpp>

class Device;
class GraphicsPipeline;
class RenderPass;
class SwapChain;

class CommandBuffer
{
    public:
        void commandbuffer_initialization(Device* device, GraphicsPipeline* graphicspipeline, RenderPass* renderpass, SwapChain* swapchain);
        void create_command_pool();
        void create_command_buffer();
        void record_command_buffer(uint32_t imageIndex);
        void destroy();

        const VkCommandBuffer& get_command_buffer();
        const VkCommandPool& get_command_pool();
    private:
        VkCommandPool m_command_pool = VK_NULL_HANDLE;
        VkCommandBuffer m_command_buffer = VK_NULL_HANDLE;

        /* helper objects */
        Device* m_temp_device = nullptr;
        GraphicsPipeline* m_temp_graphicspipeline = nullptr;
        RenderPass* m_temp_renderpass = nullptr;
        SwapChain* m_temp_swapchain = nullptr;

};