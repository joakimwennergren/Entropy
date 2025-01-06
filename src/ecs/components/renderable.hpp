#pragma once

#include "graphics/vulkan/buffers/indexbuffer.hpp"
#include "graphics/vulkan/buffers/vertexbuffer.hpp"
#include <cstdint>


namespace Entropy::Components {
    struct Renderable {
        uint32_t id = 0;
        bool visible = true;
        std::shared_ptr<VertexBuffer> vertexBuffer =
                nullptr;
        std::shared_ptr<IndexBuffer<uint16_t> > indexBuffer = nullptr;
        std::vector<uint16_t> indices = {};
        std::vector<Vertex> vertices = {};
        int zIndex = 0;
        int type = 0;
    };
} // namespace Entropy::Components

