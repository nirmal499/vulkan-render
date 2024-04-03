#include <vulkan/instance/instance.hpp>

const VkInstance& Instance::get_object()
{
    if( m_instance == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Vulkan instance is NULL");
    }

    return m_instance;
}

void Instance::create()
{
    if (COMMON::ENABLE_VALIDATION_LAYER && !COMMON::check_validation_layer_support()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = COMMON::get_required_extensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    /*
        The debugCreateInfo variable is placed outside the if statement to ensure
        that it is not destroyed before the vkCreateInstance call. By creating an
        additional debug messenger this way it will automatically be used during
        vkCreateInstance and vkDestroyInstance and cleaned up after that
    */
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (COMMON::ENABLE_VALIDATION_LAYER) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(COMMON::required_validation_layers_vec.size());
        createInfo.ppEnabledLayerNames = COMMON::required_validation_layers_vec.data();

        COMMON::populate_debug_messenger_create_info(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;

    } else {

        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance");
    }
    else
    {
        std::cout << "Successfully created vKInstance\n";
    }
}

void Instance::destroy()
{
    std::cout << "Destroying vKInstance...\n";
    vkDestroyInstance(m_instance, nullptr);
}