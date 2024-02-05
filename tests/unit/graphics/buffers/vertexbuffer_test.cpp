#include <catch2/catch_test_macros.hpp>

#include <graphics/buffers/vertexbuffer.hpp>
#include <graphics/data/vertex.hpp>
#include "setup.hpp"

TEST_CASE("VertexBuffer can be created", "[VertexBuffer]")
{
    auto serviceLocator = Setup();

    std::vector<Vertex> vertices = {
        {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    auto vertexBuffer = VertexBuffer(serviceLocator, vertices);
}