#include <vulkan/surface/surface.hpp>

void Surface::create(Instance* instance, GLFWwindow* window)
{
    if (glfwCreateWindowSurface(instance->get_object(), window, nullptr, &m_surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    else
    {
        std::cout << "Successfully created surface\n";
    }
}

void Surface::destroy(Instance* instance)
{
    vkDestroySurfaceKHR(instance->get_object(), m_surface, nullptr);
}

const VkSurfaceKHR& Surface::get_object()
{
    if(m_surface == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Surface is NULL");
    }
    return m_surface;
}