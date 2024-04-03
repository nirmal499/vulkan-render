#pragma once

#include <common/header.hpp>

class Instance;

class Surface
{   
    public:
        void surface_initialization(Instance* instance, GLFWwindow* window);
        void create();

        void destroy();

        const VkSurfaceKHR& get_object();

    private:
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;

        /* helper objects */
        Instance* m_temp_instance = nullptr;
        GLFWwindow* m_temp_window = nullptr;
};