#include <common/header.hpp>
#include <vulkan/swapchain/swapchain.hpp>
#include <vulkan/renderpass/renderpass.hpp>
#include <vulkan/graphicspipeline/graphicspipeline.hpp>
#include <vulkan/commandbuffer/commandbuffer.hpp>
#include <vulkan/vertexbuffer/vertexbuffer.hpp>

namespace COMMON
{
    void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo = {}; // Initialization is IMP
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = COMMON::debugCallback;
    }

    bool check_validation_layer_support()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : COMMON::required_validation_layers_vec) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    std::vector<const char*> get_required_extensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (COMMON::ENABLE_VALIDATION_LAYER) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    bool checkDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(COMMON::required_device_extensions_vec.begin(), COMMON::required_device_extensions_vec.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    std::vector<char> readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }


    void record_command_buffer(const VkCommandBuffer& vulkan_command_buffer, GraphicsPipeline* graphicspipeline, RenderPass* renderpass, SwapChain* swapchain, VertexBuffer* vertexbuffer, uint32_t imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(vulkan_command_buffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }else
        {
            std::cout << "Started recording command buffer\n";
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderpass->get_object();
        renderPassInfo.framebuffer = swapchain->get_frame_buffer_using_index(imageIndex);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapchain->get_swap_chain_extent();

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(vulkan_command_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(vulkan_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicspipeline->get_graphics_pipeline());

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float) swapchain->get_swap_chain_extent().width;
            viewport.height = (float) swapchain->get_swap_chain_extent().height;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(vulkan_command_buffer, 0, 1, &viewport);

            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = swapchain->get_swap_chain_extent();
            vkCmdSetScissor(vulkan_command_buffer, 0, 1, &scissor);

            VkBuffer vertexBuffers[] = {vertexbuffer->get_vertex_buffer()};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(vulkan_command_buffer, 0, 1, vertexBuffers, offsets);         

            vkCmdDraw(vulkan_command_buffer, static_cast<uint32_t>(COMMON::vertices.size()), 1, 0, 0);

        vkCmdEndRenderPass(vulkan_command_buffer);

        if (vkEndCommandBuffer(vulkan_command_buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
        else{
            std::cout << "Successfully Recorded Command Buffer\n";
        }
    }

    
}