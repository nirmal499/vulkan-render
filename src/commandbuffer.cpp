#include <vulkan/commandbuffer/commandbuffer.hpp>

#include <vulkan/device/device.hpp>
#include <vulkan/graphicspipeline/graphicspipeline.hpp>
#include <vulkan/renderpass/renderpass.hpp>
#include <vulkan/swapchain/swapchain.hpp>

void CommandBuffer::commandbuffer_initialization(Device* device, GraphicsPipeline* graphicspipeline, RenderPass* renderpass, SwapChain* swapchain)
{
    if(device == nullptr || graphicspipeline == nullptr || renderpass == nullptr || swapchain == nullptr)
    {
        throw std::runtime_error("In CommandBuffer::commandbuffer_initialization you have provided NULL");
    }

    m_temp_device = device;
    m_temp_graphicspipeline = graphicspipeline;
    m_temp_renderpass = renderpass;
    m_temp_swapchain = swapchain;
}

void CommandBuffer::create_command_pool()
{
    COMMON::QueueFamilyIndices queueFamilyIndices = m_temp_device->findQueueFamilies(m_temp_device->get_physical_device());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(m_temp_device->get_logical_device(), &poolInfo, nullptr, &m_command_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
    else
    {
        std::cout << "Successfully created CommandPool\n";
    }
}

void CommandBuffer::record_command_buffer(uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(m_command_buffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }else
    {
        std::cout << "Started recording command buffer\n";
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_temp_renderpass->get_object();
    renderPassInfo.framebuffer = m_temp_swapchain->get_frame_buffer_using_index(imageIndex);
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_temp_swapchain->get_swap_chain_extent();

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(m_command_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_temp_graphicspipeline->get_graphics_pipeline());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) m_temp_swapchain->get_swap_chain_extent().width;
        viewport.height = (float) m_temp_swapchain->get_swap_chain_extent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_command_buffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = m_temp_swapchain->get_swap_chain_extent();
        vkCmdSetScissor(m_command_buffer, 0, 1, &scissor);            

        vkCmdDraw(m_command_buffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(m_command_buffer);

    if (vkEndCommandBuffer(m_command_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
    else{
        std::cout << "Successfully Recorded Command Buffer\n";
    }
}

const VkCommandBuffer& CommandBuffer::get_command_buffer()
{
    if(m_command_buffer == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Command Buffer is NULL");
    }

    return m_command_buffer;
}

const VkCommandPool& CommandBuffer::get_command_pool()
{
    if(m_command_pool == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Command Pool is NULL");
    }

    return m_command_pool;
}

void CommandBuffer::create_command_buffer()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_command_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(m_temp_device->get_logical_device(), &allocInfo, &m_command_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
    else
    {
        std::cout << "Successfully created Command Buffer\n";
    }
}

void CommandBuffer::destroy()
{
    std::cout << "Destroying CommandPool\n";
    vkDestroyCommandPool(m_temp_device->get_logical_device(), m_command_pool, nullptr);
}