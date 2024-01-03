#pragma once

#include "mesh.hpp"
#include "skin.hpp"
#include "bounding_box.hpp"

namespace Entropy
{
    namespace GLTF
    {
        struct Node
        {
            Node *parent;
            uint32_t index;
            std::vector<Node *> children;
            glm::mat4 matrix;
            std::string name;
            Mesh *mesh;
            Skin *skin;
            int32_t skinIndex = -1;
            glm::vec3 translation{};
            glm::vec3 scale{1.0f};
            glm::quat rotation{};
            BoundingBox bvh;
            BoundingBox aabb;
            glm::mat4 localMatrix();
            glm::mat4 getMatrix();
            void update();
            ~Node();
        };
    }
}