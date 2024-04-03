#pragma once

#include <common/header.hpp>

class Instance;
class Surface;

class Device
{   
    public:
        void device_initialization(Instance* instance, Surface* surface, GLFWwindow* window);
        void pick_physical_device();

        void create_logical_device();

        const VkDevice& get_logical_device();
        const VkPhysicalDevice& get_physical_device();

        const VkQueue& get_graphics_queue();
        const VkQueue& get_present_queue();

        void destroy();

        COMMON::SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        COMMON::QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    private:
        bool isDeviceSuitable(VkPhysicalDevice device);

    private:
        VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
        VkDevice m_logical_device = VK_NULL_HANDLE;
        VkQueue m_graphics_queue = VK_NULL_HANDLE;
        VkQueue m_present_queue = VK_NULL_HANDLE;

        /* helper objects */
        Surface* m_temp_surface = nullptr;
        GLFWwindow* m_temp_window = nullptr;
        Instance* m_temp_instance = nullptr;
};