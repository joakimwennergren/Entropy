#pragma once
#include <flecs/flecs.h>

namespace Entropy
{
    namespace ECS
    {
        class World
        {
        public:
            flecs::world gameWorld;
        };
    }
}