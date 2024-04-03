#include <app/TriangleApplication.hpp>
#include <vulkan/instance/instance.hpp>

void TriangleApplication::run()
{   
    /* Allocate window resources */
    this->initialize_window();

    this->initialize_vulkan();

    this->main_loop();

    /* Deallocate window resources */
    this->cleanup();
}

void TriangleApplication::initialize_window()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(COMMON::WIDTH, COMMON::HEIGHT, "Vulkan", nullptr, nullptr);
}

void TriangleApplication::initialize_vulkan()
{
    m_instance = new Instance();
    m_instance->create();
    (void)m_instance->get_object();

    m_validation = new Validation();
    m_validation->create_debug_messenger(m_instance);
    (void)m_validation->get_object();
}

void TriangleApplication::main_loop()
{
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
    }
}

void TriangleApplication::cleanup()
{   
    m_validation->destroy(m_instance);
    delete m_validation;

    m_instance->destroy();
    delete m_instance;

    glfwDestroyWindow(m_window);

    glfwTerminate();
}