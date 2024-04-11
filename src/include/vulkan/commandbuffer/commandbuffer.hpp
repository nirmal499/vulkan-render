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
        void create_command_buffers();
        void destroy();

        const VkCommandBuffer& get_command_buffer_from_vec(size_t index);
        const VkCommandPool& get_command_pool();
    private:
        VkCommandPool m_command_pool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> m_command_buffers;

        /* helper objects */
        Device* m_temp_device = nullptr;

};