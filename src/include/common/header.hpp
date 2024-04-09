#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>
#include <fstream>

#define SHADERS_PATH "/home/nbaskey/Desktop/nirmal/projects/using-vulkan/shaders/SPIRV/"

class CommandBuffer;
class GraphicsPipeline;
class RenderPass;
class SwapChain;

namespace COMMON
{
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    const std::vector<const char*> required_validation_layers_vec = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> required_device_extensions_vec = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    #ifdef NDEBUG
        const bool ENABLE_VALIDATION_LAYER = false;
    #else
        const bool ENABLE_VALIDATION_LAYER = true;
    #endif

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
    {
            
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }

    void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    bool check_validation_layer_support();

    std::vector<const char*> get_required_extensions();

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    std::vector<char> readFile(const std::string& filename);

    void record_command_buffer(CommandBuffer* commandbuffer, GraphicsPipeline* graphicspipeline, RenderPass* renderpass, SwapChain* swapchain, uint32_t imageIndex);

}