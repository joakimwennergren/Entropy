#pragma once

#include <string>
#include <renderables/renderable.hpp>
#include <services/service.hpp>

using namespace Entropy::Renderables;
using namespace Entropy::Services;

namespace Entropy
{
    namespace SceneGraphs
    {
        class SceneGraph : public Service
        {
        public:
            SceneGraph();
            std::vector<std::shared_ptr<Renderable>> renderables;
            inline bool isValid() override { return true; };

        private:
        };
    }
}
