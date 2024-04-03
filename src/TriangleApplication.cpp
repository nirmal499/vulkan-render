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
    m_validation->validation_initialization(m_instance);
    m_validation->create_debug_messenger();
    (void)m_validation->get_object();

    m_surface = new Surface();
    m_surface->surface_initialization(m_instance, m_window);
    m_surface->create();
    (void)m_surface->get_object();

    m_device = new Device();
    m_device->device_initialization(m_instance, m_surface, m_window);
    m_device->pick_physical_device();
    m_device->create_logical_device();
    (void)m_device->get_logical_device();

    m_swapchain = new SwapChain();
    m_swapchain->swapchain_initialization(m_surface, m_window, m_device);
    m_swapchain->create_swap_chain();
    m_swapchain->create_image_views();
    (void)m_swapchain->get_object();

    m_renderpass = new RenderPass();
    m_renderpass->renderpass_initialization(m_device, m_swapchain);
    m_renderpass->create_render_pass();
    (void)m_renderpass->get_object();

    m_graphicspipeline = new GraphicsPipeline();
    m_graphicspipeline->graphicspipeline_initialization(m_device, m_renderpass);
    m_graphicspipeline->create_graphics_pipeline();
    (void)m_graphicspipeline->get_pipeline_layout();
    (void)m_graphicspipeline->get_graphics_pipeline();

    m_swapchain->create_frame_buffers(m_renderpass);
}

void TriangleApplication::main_loop()
{
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
    }
}

void TriangleApplication::cleanup()
{   
    /* We should delete the framebuffers before the image views and render pass that they are based on*/
    m_swapchain->destroy_frame_buffer(); /* Destroying FrameBuffers */
    
    m_graphicspipeline->destroy(); /* Destroying GraphicsPipeline:GraphicsPipelineLayout:RenderPass */
    m_swapchain->destroy(); /* Destroying ImageViews:SwapChain */
    m_device->destroy(); /* Destroying NOTHING */
    m_surface->destroy(); /* Destroying Surface */
    m_validation->destroy(); /* Destroying validation layer if ENABLED */
    m_instance->destroy(); /* Destroying vulkan Instance */

    delete m_graphicspipeline;
    delete m_swapchain;
    delete m_device;
    delete m_surface;
    delete m_validation;
    delete m_instance;

    glfwDestroyWindow(m_window);

    glfwTerminate();
}