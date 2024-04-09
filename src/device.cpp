#include <vulkan/device/device.hpp>

#include <vulkan/instance/instance.hpp>
#include <vulkan/surface/surface.hpp>

void Device::destroy()
{
    std::cout << "In Device there is NOTHING to destroy..\n";
}

const VkQueue& Device::get_graphics_queue()
{
    if(m_graphics_queue == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Device graphics queue is NULL");
    }

    return m_graphics_queue;
}

const VkQueue& Device::get_present_queue()
{
    if(m_present_queue == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Device present queue is NULL");
    }

    return m_present_queue;
}

void Device::device_initialization(Instance* instance, Surface* surface, GLFWwindow* window)
{
    if(instance == nullptr || surface == nullptr || window == nullptr)
    {
        throw std::runtime_error("In Device::device_initialization you have provided NULL");
    }

    m_temp_window = window;
    m_temp_surface = surface;
    m_temp_instance = instance;
}


const VkDevice& Device::get_logical_device()
{
    return m_logical_device;
}

const VkPhysicalDevice& Device::get_physical_device()
{
    return m_physical_device;
}

void Device::create_logical_device()
{
    if(m_physical_device == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Physical Device is NULL; Physical device was not selected");
    }

    COMMON::QueueFamilyIndices indices = this->findQueueFamilies(m_physical_device);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{}; // Initializing is IMP
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(COMMON::required_device_extensions_vec.size());
    createInfo.ppEnabledExtensionNames = COMMON::required_device_extensions_vec.data();

    if (COMMON::ENABLE_VALIDATION_LAYER) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(COMMON::required_validation_layers_vec.size());
        createInfo.ppEnabledLayerNames = COMMON::required_validation_layers_vec.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(m_physical_device, &createInfo, nullptr, &m_logical_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    else
    {
        std::cout << "Successfully created logical device\n";
    }

    vkGetDeviceQueue(m_logical_device, indices.graphicsFamily.value(), 0, &m_graphics_queue);
    vkGetDeviceQueue(m_logical_device, indices.presentFamily.value(), 0, &m_present_queue);
}

COMMON::SwapChainSupportDetails Device::querySwapChainSupport(VkPhysicalDevice device)
{
    COMMON::SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_temp_surface->get_object(), &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_temp_surface->get_object(), &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_temp_surface->get_object(), &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_temp_surface->get_object(), &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_temp_surface->get_object(), &presentModeCount, details.presentModes.data());
    }

    return details;
}

void Device::pick_physical_device()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_temp_instance->get_object(), &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_temp_instance->get_object(), &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (this->isDeviceSuitable(device)) {
            m_physical_device = device;
            break;
        }
    }

    if (m_physical_device == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

bool Device::isDeviceSuitable(VkPhysicalDevice device){
    /*  
        In Vulkan, anything from drawing to uploading textures, 
        requires commands to be submitted to a queue
        We need to check which queue families are supported by the device and which
        one of these supports the commands that we want to use. For that purpose we'll
        add a new function findQueueFamilies that looks for all the queue families we
        need
    */
    COMMON::QueueFamilyIndices indices = this->findQueueFamilies(device);
    bool extensionsSupported = COMMON::checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        COMMON::SwapChainSupportDetails swapChainSupport_details = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport_details.formats.empty() && !swapChainSupport_details.presentModes.empty();
        if(swapChainAdequate)
        {
            std::cout << "SwapChain is supported\n";
        }
    }

    return swapChainAdequate && indices.isComplete() && extensionsSupported;
}

COMMON::QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice device)
{
    COMMON::QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_temp_surface->get_object(), &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}