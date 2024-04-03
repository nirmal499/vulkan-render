#pragma once

#include <common/header.hpp>
#include <vulkan/instance/instance.hpp>

class Surface
{   
    public:
        void create(Instance* instance, GLFWwindow* window);

        void destroy(Instance* instance);

        const VkSurfaceKHR& get_object();

    private:
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
};