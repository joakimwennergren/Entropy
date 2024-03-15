#pragma once
#include <future>
#include <gltf/model.hpp>

namespace Entropy
{
    namespace Components
    {
        struct Model
        {
            Entropy::GLTF::Model *model;
        };
    }
}