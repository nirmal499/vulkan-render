#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

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
class VertexBuffer;

namespace COMMON
{
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    const int MAX_FRAMES_IN_FLIGHT = 2;

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};

            /* 
                All of our per-vertex data is packed together in one array, so we’re only going
                to have one binding
            */
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(COMMON::Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(COMMON::Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(COMMON::Vertex, color);

            return attributeDescriptions;
        }
    };

    /*
    const std::vector<Vertex> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
    */

    const std::vector<Vertex> vertices = {
        {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

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

    void record_command_buffer(const VkCommandBuffer& vulkan_command_buffer, GraphicsPipeline* graphicspipeline, RenderPass* renderpass, SwapChain* swapchain, VertexBuffer* vertexbuffer, uint32_t imageIndex);

}