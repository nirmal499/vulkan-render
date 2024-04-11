#include <vulkan/commandbuffer/commandbuffer.hpp>

#include <vulkan/device/device.hpp>
#include <vulkan/graphicspipeline/graphicspipeline.hpp>
#include <vulkan/renderpass/renderpass.hpp>
#include <vulkan/swapchain/swapchain.hpp>

void CommandBuffer::commandbuffer_initialization(Device* device)
{
    if(device == nullptr)
    {
        throw std::runtime_error("In CommandBuffer::commandbuffer_initialization you have provided NULL");
    }

    m_temp_device = device;
}

void CommandBuffer::create_command_pool()
{
    COMMON::QueueFamilyIndices queueFamilyIndices = m_temp_device->findQueueFamilies(m_temp_device->get_physical_device());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(m_temp_device->get_logical_device(), &poolInfo, nullptr, &m_command_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
    else
    {
        std::cout << "Successfully created CommandPool\n";
    }
}

const VkCommandBuffer& CommandBuffer::get_command_buffer_from_vec(size_t index)
{
    return m_command_buffers.at(index);
}

const VkCommandPool& CommandBuffer::get_command_pool()
{
    if(m_command_pool == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Command Pool is NULL");
    }

    return m_command_pool;
}

void CommandBuffer::create_command_buffers()
{
    m_command_buffers.resize(COMMON::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_command_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_command_buffers.size());

    if (vkAllocateCommandBuffers(m_temp_device->get_logical_device(), &allocInfo, m_command_buffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
    else
    {
        std::cout << "Successfully created Command Buffer\n";
    }
}

void CommandBuffer::destroy()
{
    std::cout << "Destroying CommandPool\n";
    vkDestroyCommandPool(m_temp_device->get_logical_device(), m_command_pool, nullptr);
}