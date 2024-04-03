#pragma once

#include <common/header.hpp>

class Device;

class SyncObject
{
    public:
        void syncobject_initialization(Device* device);
        void create_sync_object();
        void destroy();

        const VkSemaphore& get_image_available_semaphore();
        const VkSemaphore& get_render_finished_semaphore();
        const VkFence& get_inflight_fence();
    private:
        VkSemaphore m_image_available_semaphore = VK_NULL_HANDLE;
        VkSemaphore m_render_finished_semaphore = VK_NULL_HANDLE;
        VkFence m_inflight_fence = VK_NULL_HANDLE;

        /* helper objects */
        Device* m_temp_device = nullptr;
};