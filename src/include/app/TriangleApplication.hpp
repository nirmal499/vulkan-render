#pragma once

#include <common/header.hpp>
#include <vulkan/instance/instance.hpp>
#include <vulkan/validation_layer/validation.hpp>

class TriangleApplication
{
    public:
        void run();
    private:
        void initialize_window();
        void initialize_vulkan();
        void main_loop();
        void cleanup();

    private:
        GLFWwindow* m_window = nullptr;
        Instance* m_instance = nullptr;
        Validation* m_validation = nullptr;
};