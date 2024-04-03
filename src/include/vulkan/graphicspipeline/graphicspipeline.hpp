#pragma once

#include <common/header.hpp>
#include <vulkan/device/device.hpp>

class GraphicsPipeline
{
    public:
        const VkPipelineLayout& get_object();

        void create(Device* device);

        void destroy();
    private:
        VkShaderModule createShaderModule(const std::vector<char>& code);

    private:
        VkPipelineLayout m_pipeline_layout;

        /* helper objects */
        Device* m_temp_device;
};