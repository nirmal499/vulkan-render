#include <app/TriangleApplication.hpp>

void TriangleApplication::run()
{   
    /* Allocate window resources */
    this->initialize_window();

    this->initialize_vulkan();

    m_current_frame = 0;

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

    m_commandbuffer = new CommandBuffer();
    m_commandbuffer->commandbuffer_initialization(m_device);
    m_commandbuffer->create_command_pool();
    m_commandbuffer->create_command_buffers();
    (void)m_commandbuffer->get_command_pool();

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

    m_syncobject = new SyncObject();
    m_syncobject->syncobject_initialization(m_device);
    m_syncobject->create_sync_objects();
}

void TriangleApplication::main_loop()
{
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();

        this->drawFrame();
    }

    vkDeviceWaitIdle(m_device->get_logical_device());
}

void TriangleApplication::drawFrame()
{
    vkWaitForFences(m_device->get_logical_device(), 1, &m_syncobject->get_inflight_fence_from_vec(m_current_frame), VK_TRUE, UINT64_MAX);
    vkResetFences(m_device->get_logical_device(), 1, &m_syncobject->get_inflight_fence_from_vec(m_current_frame));

    uint32_t imageIndex;
    vkAcquireNextImageKHR(m_device->get_logical_device(), m_swapchain->get_object(), UINT64_MAX, m_syncobject->get_image_available_semaphore_from_vec(m_current_frame), VK_NULL_HANDLE, &imageIndex);

    vkResetCommandBuffer(m_commandbuffer->get_command_buffer_from_vec(m_current_frame), /*VkCommandBufferResetFlagBits*/ 0);
    COMMON::record_command_buffer(
        m_commandbuffer->get_command_buffer_from_vec(m_current_frame), m_graphicspipeline, m_renderpass, m_swapchain, imageIndex
    );

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {m_syncobject->get_image_available_semaphore_from_vec(m_current_frame)};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandbuffer->get_command_buffer_from_vec(m_current_frame);

    VkSemaphore signalSemaphores[] = {m_syncobject->get_render_finished_semaphore_from_vec(m_current_frame)};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(m_device->get_graphics_queue(), 1, &submitInfo, m_syncobject->get_inflight_fence_from_vec(m_current_frame)) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {m_swapchain->get_object()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(m_device->get_present_queue(), &presentInfo);

    m_current_frame = (m_current_frame + 1) % COMMON::MAX_FRAMES_IN_FLIGHT;
}

void TriangleApplication::cleanup()
{   

    m_syncobject->destroy(); /* Destroying SyncObjects(Semaphores) */
    m_commandbuffer->destroy(); /* Destroying CommandPool */

    /* We should delete the framebuffers before the image views and render pass that they are based on*/
    m_swapchain->destroy_frame_buffer(); /* Destroying SwapChainFrameBuffers */
    
    m_graphicspipeline->destroy(); /* Destroying GraphicsPipeline:GraphicsPipelineLayout */
    m_renderpass->destroy(); /* Destroying RenderPass */
    m_swapchain->destroy(); /* Destroying SwapChainImageViews:SwapChain */
    m_device->destroy(); /* Destroying NOTHING */
    m_surface->destroy(); /* Destroying Surface */
    m_validation->destroy(); /* Destroying validation layer if ENABLED */
    m_instance->destroy(); /* Destroying vulkan Instance */

    delete m_syncobject;
    delete m_commandbuffer;
    delete m_graphicspipeline;
    delete m_renderpass;
    delete m_swapchain;
    delete m_device;
    delete m_surface;
    delete m_validation;
    delete m_instance;

    glfwDestroyWindow(m_window);

    glfwTerminate();
}