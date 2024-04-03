#pragma once

#include <common/header.hpp>

class Device;
class RenderPass;

class GraphicsPipeline
{
    public:
        const VkPipelineLayout& get_pipeline_layout();
        const VkPipeline& get_graphics_pipeline();

        void graphicspipeline_initialization(Device* device, RenderPass* renderpass);
        void create_graphics_pipeline();

        void destroy();
    private:
        VkShaderModule createShaderModule(const std::vector<char>& code);

    private:
        VkPipeline m_graphics_pipeline = VK_NULL_HANDLE;
        VkPipelineLayout m_pipeline_layout = VK_NULL_HANDLE;

        /* helper objects */
        Device* m_temp_device = nullptr;
        RenderPass* m_temp_renderpass = nullptr;
};