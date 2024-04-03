#include <vulkan/renderpass/renderpass.hpp>
/* We included the headers in .cpp inorder to solve the recursive header dependency problem */
#include <vulkan/device/device.hpp>
#include <vulkan/swapchain/swapchain.hpp>

void RenderPass::renderpass_initialization(Device* device, SwapChain* swapchain)
{
    if(device == nullptr || swapchain == nullptr)
    {
        throw std::runtime_error("In RenderPass::renderpass_initialization you have provided NULL");
    }

    m_temp_swapchain = swapchain;
    m_temp_device = device;
}


const VkRenderPass& RenderPass::get_object()
{
    if(m_render_pass == VK_NULL_HANDLE)
    {
        throw std::runtime_error("RenderPass is NULL");
    }
    return m_render_pass;
}

void RenderPass::destroy()
{
    std::cout << "Destroying RenderPass...\n";
    vkDestroyRenderPass(m_temp_device->get_logical_device(), m_render_pass, nullptr);
}

void RenderPass::create_render_pass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = m_temp_swapchain->get_swap_chain_image_format();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(m_temp_device->get_logical_device(), &renderPassInfo, nullptr, &m_render_pass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
    else
    {
        std::cout << "Successfully created Render Pass\n";
    }
}