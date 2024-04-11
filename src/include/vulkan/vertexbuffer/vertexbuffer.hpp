#pragma once

#include <common/header.hpp>

class Device;

class VertexBuffer
{
    public:
        void vertexbuffer_initialization(Device* device);
        void create_vertex_buffer();

        const VkBuffer& get_vertex_buffer();

        void destroy();

    private:
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    private:
        VkBuffer m_vertex_buffer = VK_NULL_HANDLE;
        VkDeviceMemory m_vertex_buffer_memory = VK_NULL_HANDLE;

        Device* m_temp_device = nullptr;
};