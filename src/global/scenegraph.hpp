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
            SceneGraph(SceneGraph &other) = delete;
            void operator=(const SceneGraph &) = delete;
            static SceneGraph *GetInstance();
            
            std::vector<std::shared_ptr<Renderable>> renderables;
        };
    }
}
