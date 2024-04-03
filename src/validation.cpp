#include <vulkan/validation_layer/validation.hpp>
#include <vulkan/instance/instance.hpp>

void Validation::validation_initialization(Instance* instance)
{
    if(instance == nullptr)
    {
        throw std::runtime_error("Validation::validation_initialization is provided with NULL objects");
    }

    m_temp_instance = instance;
}

const VkDebugUtilsMessengerEXT& Validation::get_object()
{
    if(COMMON::ENABLE_VALIDATION_LAYER && m_debugMessenger == VK_NULL_HANDLE)
    {
        throw std::runtime_error("DebugMessenger Object is NULL");
    }

    return m_debugMessenger;
}

void Validation::destroy()
{
    if (COMMON::ENABLE_VALIDATION_LAYER) {
        std::cout << "Destroying Debug Layer...\n";
        this->DestroyDebugUtilsMessengerEXT(m_temp_instance->get_object(), nullptr);
    }
}

void Validation::create_debug_messenger()
{
    if (!COMMON::ENABLE_VALIDATION_LAYER) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{}; // Initialization is IMP
    COMMON::populate_debug_messenger_create_info(createInfo);

    if (this->CreateDebugUtilsMessengerEXT(m_temp_instance->get_object(), &createInfo, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create debug messenger");
    }
    else
    {
        std::cout << "Successfully created created debug object and attached it to VkInstance\n";
    }
}