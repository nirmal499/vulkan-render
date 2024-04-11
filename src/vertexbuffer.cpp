#include <vulkan/vertexbuffer/vertexbuffer.hpp>
#include <vulkan/device/device.hpp>

void VertexBuffer::vertexbuffer_initialization(Device* device)
{
    if(device == nullptr)
    {
        throw std::runtime_error("In VertexBuffer::vertexbuffer_initialization you have NULL objects");
    }

    m_temp_device = device;
}

void VertexBuffer::create_vertex_buffer()
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(COMMON::vertices[0]) * COMMON::vertices.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_temp_device->get_logical_device(), &bufferInfo, nullptr, &m_vertex_buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex buffer!");
    }
    else
    {
        std::cout << "Successfully created vertex buffer\n";
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_temp_device->get_logical_device(), m_vertex_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = this->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(m_temp_device->get_logical_device(), &allocInfo, nullptr, &m_vertex_buffer_memory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }
    else
    {
        std::cout << "Successfully allocated vertex buffer memory\n";
    }

    vkBindBufferMemory(m_temp_device->get_logical_device(), m_vertex_buffer, m_vertex_buffer_memory, 0);

    void* data;

    vkMapMemory(m_temp_device->get_logical_device(), m_vertex_buffer_memory, 0, bufferInfo.size, 0, &data);
        memcpy(data, COMMON::vertices.data(), (size_t) bufferInfo.size);
    vkUnmapMemory(m_temp_device->get_logical_device(), m_vertex_buffer_memory);
}

uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_temp_device->get_physical_device(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

const VkBuffer& VertexBuffer::get_vertex_buffer()
{
    return m_vertex_buffer;
}

void VertexBuffer::destroy()
{
    vkDestroyBuffer(m_temp_device->get_logical_device(), m_vertex_buffer, nullptr);
    vkFreeMemory(m_temp_device->get_logical_device(), m_vertex_buffer_memory, nullptr);
}