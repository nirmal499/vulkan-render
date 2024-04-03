#pragma once

#include <common/header.hpp>

class Surface;
class Device;
class RenderPass;

class SwapChain
{
    public:
        void swapchain_initialization(Surface* surface, GLFWwindow* window, Device* device);
        void create_swap_chain();
        void create_image_views();

        void create_frame_buffers(RenderPass* renderpass);
        void destroy_frame_buffer();

        void destroy();

        const VkSwapchainKHR& get_object();
        const VkFormat& get_swap_chain_image_format();
        const VkExtent2D& get_swap_chain_extent();

        const VkFramebuffer& get_frame_buffer_using_index(uint32_t imageIndex);

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
        std::vector<VkFramebuffer> m_swap_chain_frame_buffers;

        /* helper objects */
        Device* m_temp_device = nullptr;
        Surface* m_temp_surface = nullptr;
        GLFWwindow* m_temp_window = nullptr;
};