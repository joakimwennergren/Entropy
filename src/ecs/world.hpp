#pragma once
#include <flecs/flecs.h>
#include <services/service.hpp>

namespace Entropy
{
    namespace ECS
    {
        class World : public Service
        {
        public:
            flecs::world gameWorld;
            flecs::world gizmoWorld;
            std::vector<flecs::entity> _entities;
        };
    }
}