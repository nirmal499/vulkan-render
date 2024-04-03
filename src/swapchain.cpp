#include <vulkan/swapchain/swapchain.hpp>

const VkSwapchainKHR& SwapChain::get_object()
{
    if(m_swap_chain == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Swapchain object is NULL");
    }
    return m_swap_chain;
}

void SwapChain::destroy()
{
    for (auto imageView : m_swap_chain_image_views) 
    {
        vkDestroyImageView(m_temp_device->get_logical_device(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(m_temp_device->get_logical_device(), m_swap_chain, nullptr);
}

void SwapChain::create_image_views()
{
    m_swap_chain_image_views.resize(m_swap_chain_images.size());

    for (size_t i = 0; i < m_swap_chain_images.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_swap_chain_images[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_swap_chain_image_format;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(m_temp_device->get_logical_device(), &createInfo, nullptr, &m_swap_chain_image_views[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }

    std::cout << "Successfully created " << m_swap_chain_images.size() << " ImageViews\n";

}

void SwapChain::create_swap_chain(Surface* surface, GLFWwindow* window, Device* device)
{

    if(surface == nullptr || window == nullptr || device == nullptr)
    {
        throw std::runtime_error("In SwapChain::create_swap_chain you provided NULL object");
    }

    m_temp_device = device;
    m_temp_surface = surface;
    m_temp_window = window;

    COMMON::SwapChainSupportDetails swapChainSupport_details = device->querySwapChainSupport(device->get_physical_device());

    VkSurfaceFormatKHR surfaceFormat = this->chooseSwapSurfaceFormat(swapChainSupport_details.formats);
    VkPresentModeKHR presentMode = this->chooseSwapPresentMode(swapChainSupport_details.presentModes);
    VkExtent2D extent = this->chooseSwapExtent(swapChainSupport_details.capabilities);

    uint32_t imageCount = swapChainSupport_details.capabilities.minImageCount + 1;
    if (swapChainSupport_details.capabilities.maxImageCount > 0 && imageCount > swapChainSupport_details.capabilities.maxImageCount) {
        imageCount = swapChainSupport_details.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface->get_object();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    COMMON::QueueFamilyIndices indices = device->findQueueFamilies(device->get_physical_device());
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport_details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device->get_logical_device(), &createInfo, nullptr, &m_swap_chain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }
    else
    {
        std::cout << "Successfully created Swapchain\n";
    }

    vkGetSwapchainImagesKHR(device->get_logical_device(), m_swap_chain, &imageCount, nullptr);
    m_swap_chain_images.resize(imageCount);
    vkGetSwapchainImagesKHR(device->get_logical_device(), m_swap_chain, &imageCount, m_swap_chain_images.data());

    m_swap_chain_image_format = surfaceFormat.format;
    m_swap_chain_extent = extent;
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(m_temp_window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}