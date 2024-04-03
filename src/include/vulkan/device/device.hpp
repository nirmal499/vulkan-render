#pragma once

#include <common/header.hpp>
#include <vulkan/instance/instance.hpp>
#include <vulkan/surface/surface.hpp>

class Device
{   
    public:
        void pick_physical_device(Instance* instance, Surface* surface, GLFWwindow* window);

        void create_logical_device();

        const VkDevice& get_logical_device();
        const VkPhysicalDevice& get_physical_device();

        void destroy(){}

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
};