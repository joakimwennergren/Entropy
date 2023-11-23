#pragma once

#include <string>
#include <mutex>

#include <contexts/context.hpp>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)

#include <GLFW/glfw3.h>

#endif

using namespace Symbios::Core;

namespace Entropy
{
    namespace Global
    {
        class VulkanContext
        {

        private:
            static VulkanContext *pinstance_;
            static std::mutex mutex_;

        protected:
            VulkanContext()
            {
            }

            ~VulkanContext() {}

            std::shared_ptr<Context> vulkanContext;

        public:
            VulkanContext(VulkanContext &other) = delete;

            void operator=(const VulkanContext &) = delete;

            static VulkanContext *GetInstance();

            std::shared_ptr<Context> GetVulkanContext() { return this->vulkanContext; };

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)
            void InitializeContext(GLFWwindow *window)
            {
                vulkanContext = std::make_shared<Context>(window);
            }
#else
            void InitializeContext(CA::MetalLayer *layer, CGRect frame)
            {
                vulkanContext = std::make_shared<Context>(layer, frame);
            }
#endif
        };
    }
}
