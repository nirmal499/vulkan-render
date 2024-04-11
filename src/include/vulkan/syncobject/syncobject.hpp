#pragma once

#include <common/header.hpp>

class Device;

class SyncObject
{
    public:
        void syncobject_initialization(Device* device);
        void create_sync_objects();
        void destroy();

        const VkSemaphore& get_image_available_semaphore_from_vec(size_t index);
        const VkSemaphore& get_render_finished_semaphore_from_vec(size_t index);
        const VkFence& get_inflight_fence_from_vec(size_t index);
    private:
        std::vector<VkSemaphore> m_image_available_semaphores;
        std::vector<VkSemaphore> m_render_finished_semaphores;
        std::vector<VkFence> m_inflight_fences;

        /* helper objects */
        Device* m_temp_device = nullptr;
};