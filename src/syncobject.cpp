#include <vulkan/syncobject/syncobject.hpp>
#include <vulkan/device/device.hpp>

const VkSemaphore& SyncObject::get_image_available_semaphore_from_vec(size_t index)
{
    return m_image_available_semaphores.at(index);
}
const VkSemaphore& SyncObject::get_render_finished_semaphore_from_vec(size_t index)
{
    return m_render_finished_semaphores.at(index);
}

const VkFence& SyncObject::get_inflight_fence_from_vec(size_t index)
{
    return m_inflight_fences.at(index);
}

void SyncObject::syncobject_initialization(Device* device)
{
    if(device == nullptr)
    {
        throw std::runtime_error("In SyncObject::syncobject_initialization provided with NULL objects");
    }

    m_temp_device = device;
}

void SyncObject::create_sync_objects()
{
    m_image_available_semaphores.resize(COMMON::MAX_FRAMES_IN_FLIGHT);
    m_render_finished_semaphores.resize(COMMON::MAX_FRAMES_IN_FLIGHT);
    m_inflight_fences.resize(COMMON::MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < COMMON::MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (
                vkCreateSemaphore(m_temp_device->get_logical_device(), &semaphoreInfo, nullptr, &m_image_available_semaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_temp_device->get_logical_device(), &semaphoreInfo, nullptr, &m_render_finished_semaphores[i]) != VK_SUCCESS ||
                vkCreateFence(m_temp_device->get_logical_device(), &fenceInfo, nullptr, &m_inflight_fences[i]) != VK_SUCCESS
            ) 
        {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
        else
        {
            std::cout << "Successfully created synchronization objects for a frame\n";
        }
    }
}

void SyncObject::destroy()
{
    std::cout << "Destroying SyncObjects....\n";

    for(size_t i = 0; i < COMMON::MAX_FRAMES_IN_FLIGHT; i++)
    {    
        vkDestroySemaphore(m_temp_device->get_logical_device(), m_render_finished_semaphores[i], nullptr);
        vkDestroySemaphore(m_temp_device->get_logical_device(), m_image_available_semaphores[i], nullptr);
        vkDestroyFence(m_temp_device->get_logical_device(), m_inflight_fences[i], nullptr);
    }
}