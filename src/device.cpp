#include <vulkan/device/device.hpp>

const VkDevice& Device::get_object()
{
    return m_logical_device;
}

void Device::create_logical_device()
{
    if(m_physical_device == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Physical Device is NULL; Physical device was not selected");
    }

    COMMON::QueueFamilyIndices indices = findQueueFamilies(m_physical_device);

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

    createInfo.enabledExtensionCount = 0;

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

void Device::pick_physical_device(Instance* instance, Surface* surface)
{

    if(instance == nullptr )
    {
        throw std::runtime_error("Provided Instance* is NULL");
    }

    if(surface == nullptr)
    {
        throw std::runtime_error("Provided Surface* is NULL");
    }

    m_temp_surface = surface;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance->get_object(), &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance->get_object(), &deviceCount, devices.data());

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

    return indices.isComplete();
}

COMMON::QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice device) {
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