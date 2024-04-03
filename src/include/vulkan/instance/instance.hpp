#pragma once

#include <common/header.hpp>

/* Wrapper over VkInstance */
class Instance
{
    public:

        void create();
        void destroy();

        const VkInstance& get_object();

    private:
        /* 
            The instance is the connection between your application and
            the Vulkan library and creating it involves specifying some details about your
            application to the driver.
        */
        VkInstance m_instance = VK_NULL_HANDLE;
};