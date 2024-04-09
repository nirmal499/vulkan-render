#pragma once

#include <common/header.hpp>

class Device;
class GraphicsPipeline;
class RenderPass;
class SwapChain;

class CommandBuffer
{
    public:
        void commandbuffer_initialization(Device* device);
        void create_command_pool();
        void create_command_buffer();
        void destroy();

        const VkCommandBuffer& get_command_buffer();
        const VkCommandPool& get_command_pool();
    private:
        VkCommandPool m_command_pool = VK_NULL_HANDLE;
        VkCommandBuffer m_command_buffer = VK_NULL_HANDLE;

        /* helper objects */
        Device* m_temp_device = nullptr;

};