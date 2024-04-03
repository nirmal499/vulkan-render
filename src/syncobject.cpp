#include <vulkan/syncobject/syncobject.hpp>
#include <vulkan/device/device.hpp>

const VkSemaphore& SyncObject::get_image_available_semaphore()
{
    if(m_image_available_semaphore == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Image Available Semaphore is NULL");
    }
    return m_image_available_semaphore;
}
const VkSemaphore& SyncObject::get_render_finished_semaphore()
{
    if(m_render_finished_semaphore == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Render Finished Semaphore is NULL");
    }
    return m_render_finished_semaphore;
}

const VkFence& SyncObject::get_inflight_fence()
{
    if(m_inflight_fence == VK_NULL_HANDLE)
    {
        throw std::runtime_error("InFlight Fence is NULL");
    }
    return m_inflight_fence;
}

void SyncObject::syncobject_initialization(Device* device)
{
    if(device == nullptr)
    {
        throw std::runtime_error("In SyncObject::syncobject_initialization provided with NULL objects");
    }

    m_temp_device = device;
}

void SyncObject::create_sync_object()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateSemaphore(m_temp_device->get_logical_device(), &semaphoreInfo, nullptr, &m_image_available_semaphore) != VK_SUCCESS ||
        vkCreateSemaphore(m_temp_device->get_logical_device(), &semaphoreInfo, nullptr, &m_render_finished_semaphore) != VK_SUCCESS ||
        vkCreateFence(m_temp_device->get_logical_device(), &fenceInfo, nullptr, &m_inflight_fence) != VK_SUCCESS
    ) {
        
        throw std::runtime_error("failed to create synchronization objects for a frame!");
    }
    else
    {
        std::cout << "Successfully created synchronization objects for a frame\n";
    }
}

void SyncObject::destroy()
{
    std::cout << "Destroying SyncObjects....\n";
    vkDestroySemaphore(m_temp_device->get_logical_device(), m_render_finished_semaphore, nullptr);
    vkDestroySemaphore(m_temp_device->get_logical_device(), m_image_available_semaphore, nullptr);
    vkDestroyFence(m_temp_device->get_logical_device(), m_inflight_fence, nullptr);
}