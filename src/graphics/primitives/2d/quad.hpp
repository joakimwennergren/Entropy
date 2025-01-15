#pragma once

#include "graphics/vulkan/textures/texture.hpp"
#include <data/vertex.hpp>
#include <filesystem/filesystem.hpp>
#include <graphics/vulkan/buffers/indexbuffer.hpp>
#include <graphics/vulkan/buffers/vertexbuffer.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>

using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Data;

namespace Entropy {
    namespace Graphics {
        namespace Primitives {
            struct Quad {
            public:
                Quad() {
                    texture =
                            std::make_shared<Texture>(Filesystem::GetSpritesDir() + "_blank.png");
                    vertexBuffer = std::make_shared<VertexBuffer>(vertices);
                    indexBuffer = std::make_shared<IndexBuffer<uint16_t> >(indices);
                }

                explicit Quad(std::vector<Vertex> v) {
                    texture =
                            std::make_shared<Texture>(Filesystem::GetSpritesDir() + "_blank.png");
                    vertexBuffer = std::make_shared<VertexBuffer>(v);
                    indexBuffer = std::make_shared<IndexBuffer<uint16_t> >(indices);
                }

                std::shared_ptr<IndexBuffer<uint16_t> > indexBuffer;
                std::shared_ptr<VertexBuffer> vertexBuffer;
                std::vector<Vertex> vertices = {
                    {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
                    {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
                    {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
                    {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
                };
                // Indices for the two triangles that make up the quad
                std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};
                std::shared_ptr<Texture> texture;
            };
        } // namespace Primitives
    } // namespace Graphics
} // namespace Entropy
