#pragma once

#include <gltf/model.hpp>

namespace Entropy
{
    namespace Components
    {
        struct Model
        {
            std::shared_ptr<Entropy::GLTF::Model> model;
        };
    }
}