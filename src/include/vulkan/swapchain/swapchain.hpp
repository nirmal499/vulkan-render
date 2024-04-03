#pragma once

#include <common/header.hpp>
#include <vulkan/device/device.hpp>
#include <vulkan/surface/surface.hpp>

class SwapChain
{
    public:
        void create_swap_chain(Surface* surface, GLFWwindow* window, Device* device);
        void create_image_views();
        void destroy();

        const VkSwapchainKHR& get_object();

    private:

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    private:

        VkSwapchainKHR m_swap_chain = VK_NULL_HANDLE;
        std::vector<VkImage> m_swap_chain_images;
        VkFormat m_swap_chain_image_format;
        VkExtent2D m_swap_chain_extent;

        std::vector<VkImageView> m_swap_chain_image_views;

        /* helper objects */
        Device* m_temp_device = nullptr;
        Surface* m_temp_surface = nullptr;
        GLFWwindow* m_temp_window = nullptr;
};