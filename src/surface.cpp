#include <vulkan/surface/surface.hpp>
#include <vulkan/instance/instance.hpp>

void Surface::surface_initialization(Instance* instance, GLFWwindow* window)
{
    if(instance == nullptr || window == nullptr)
    {
        throw std::runtime_error("In Surface::surface_initialization you have provided NULL");
    }

    m_temp_window = window;
    m_temp_instance = instance;
}

void Surface::create()
{
    if (glfwCreateWindowSurface(m_temp_instance->get_object(), m_temp_window, nullptr, &m_surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    else
    {
        std::cout << "Successfully created surface\n";
    }
}

void Surface::destroy()
{
    std::cout << "Destroying Surface...\n";
    vkDestroySurfaceKHR(m_temp_instance->get_object(), m_surface, nullptr);
}

const VkSurfaceKHR& Surface::get_object()
{
    if(m_surface == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Surface is NULL");
    }
    return m_surface;
}