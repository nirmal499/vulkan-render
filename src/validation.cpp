#include <vulkan/validation_layer/validation.hpp>

const VkDebugUtilsMessengerEXT& Validation::get_object()
{
    if(COMMON::ENABLE_VALIDATION_LAYER && m_debugMessenger == VK_NULL_HANDLE)
    {
        throw std::runtime_error("DebugMessenger Object is NULL");
    }

    return m_debugMessenger;
}

void Validation::destroy(Instance* instance)
{
    if (COMMON::ENABLE_VALIDATION_LAYER) {
        this->DestroyDebugUtilsMessengerEXT(instance->get_object(), nullptr);
    }
}

void Validation::create_debug_messenger(Instance* instance)
{
    if (!COMMON::ENABLE_VALIDATION_LAYER) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{}; // Initialization is IMP
    COMMON::populate_debug_messenger_create_info(createInfo);

    if (this->CreateDebugUtilsMessengerEXT(instance->get_object(), &createInfo, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create debug messenger");
    }
    else
    {
        std::cout << "Successfully created created debug object and attached it to VkInstance\n";
    }
}