#pragma once

#include <common/header.hpp>
#include <vulkan/instance/instance.hpp>

class Validation
{       
    public:
        void create_debug_messenger(Instance* instance);
        void destroy(Instance* instance);

        const VkDebugUtilsMessengerEXT& get_object();

    private:
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator) {
            auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
            if (func != nullptr) {
                /* We are passing &m_debugMessenger becoz it is expecting "VkDebugUtilsMessengerEXT*" */
                return func(instance, pCreateInfo, pAllocator, &m_debugMessenger);
            } else {
                return VK_ERROR_EXTENSION_NOT_PRESENT;
            }
        }

        void DestroyDebugUtilsMessengerEXT(VkInstance instance, const VkAllocationCallbacks* pAllocator) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                /* We are passing m_debugMessenger becoz it is expecting "VkDebugUtilsMessengerEXT" */
                func(instance, m_debugMessenger, pAllocator);
            }
        }

    private:
        VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
};