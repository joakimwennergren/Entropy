#pragma once

#include <string>
#include <mutex>

#include "renderable.hpp"

using namespace Symbios::Core;
using namespace Symbios::Renderables;

namespace Symbios
{
    namespace Global
    {
        class SceneGraph
        {
        private:
            static SceneGraph *pinstance_;
            static std::mutex mutex_;

        protected:
            SceneGraph(){}
            ~SceneGraph(){}

        public:
            std::vector<std::unique_ptr<Renderable>> renderables;
            
            SceneGraph(SceneGraph &other) = delete;
            void operator=(const SceneGraph &) = delete;

            static SceneGraph *GetInstance();
            
            void Add(std::unique_ptr<Renderable> renderable);
        };
    }
}
