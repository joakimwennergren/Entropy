#pragma once

#include <vulkan/vulkan.hpp>
#include <servicelocators/servicelocator.hpp>

struct IRenderPass : public IService
{
    virtual ~IRenderPass() = default;
    virtual VkRenderPass Get() = 0;
    virtual void RecreateFrameBuffers(int width, int height, bool app) = 0;
    virtual void RecreateDepthBuffer(int width, int height) = 0;
    virtual void Begin(CommandBuffer commandBuffer, uint32_t imageIndex, int width,
                       int height) = 0;
    virtual void End(CommandBuffer commandBuffer) = 0;
};