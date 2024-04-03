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

    m_surface = new Surface();
    m_surface->create(m_instance, m_window);
    (void)m_surface->get_object();

    m_device = new Device();
    m_device->pick_physical_device(m_instance, m_surface, m_window);
    m_device->create_logical_device();
    (void)m_device->get_logical_device();

    m_swapchain = new SwapChain();
    m_swapchain->create_swap_chain(m_surface, m_window, m_device);
    m_swapchain->create_image_views();
    (void)m_swapchain->get_object();

    m_graphicspipeline = new GraphicsPipeline();
    m_graphicspipeline->graphicspipeline_initialization(m_device, m_swapchain);
    m_graphicspipeline->create_render_pass();
    m_graphicspipeline->create_graphics_pipeline();
    (void)m_graphicspipeline->get_render_pass();
    (void)m_graphicspipeline->get_pipeline_layout();
    (void)m_graphicspipeline->get_graphics_pipeline();
}

void TriangleApplication::main_loop()
{
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
    }
}

void TriangleApplication::cleanup()
{   

    m_graphicspipeline->destroy();
    m_swapchain->destroy();
    m_device->destroy();
    m_surface->destroy(m_instance);
    m_validation->destroy(m_instance);
    m_instance->destroy();

    delete m_graphicspipeline;
    delete m_swapchain;
    delete m_device;
    delete m_surface;
    delete m_validation;
    delete m_instance;

    glfwDestroyWindow(m_window);

    glfwTerminate();
}