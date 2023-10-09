/**
 * @file state.hpp
 * @author Joakim Wennergren (joakim.wennergren@databeams.se)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <string>
#include <mutex>

#include "renderable.hpp"

using namespace Symbios::Core;
using namespace Symbios::Renderables;

namespace Symbios
{
    namespace Contexts
    {
        /**
         * The Singleton class defines the `GetInstance` method that serves as an
         * alternative to constructor and lets clients access the same instance of this
         * class over and over.
         */
        class SceneGraph
        {

            /**
             * The Singleton's constructor/destructor should always be private to
             * prevent direct construction/desctruction calls with the `new`/`delete`
             * operator.
             */
        private:
            static SceneGraph *pinstance_;
            static std::mutex mutex_;

        protected:
            SceneGraph()
            {

            }

            ~SceneGraph() {}



        public:

            std::vector<std::shared_ptr<Renderable>> renderables;
            
            /**
             * Singletons should not be cloneable.
             */
            SceneGraph(SceneGraph &other) = delete;
            /**
             * Singletons should not be assignable.
             */
            void operator=(const SceneGraph &) = delete;
            /**
             * This is the static method that controls the access to the singleton
             * instance. On the first run, it creates a singleton object and places it
             * into the static field. On subsequent runs, it returns the client existing
             * object stored in the static field.
             */

            //inline std::shared_ptr<SceneGraph> GetSceneGraph() {return this->_sceneGraph;};

            static SceneGraph *GetInstance();
            /**
             * Finally, any singleton should define some business logic, which can be
             * executed on its instance.
             */
            
            inline std::vector<std::shared_ptr<Renderable>> GetRenderables() { return this->renderables;};

            inline void Add(std::shared_ptr<Renderable> renderable)
            {
                PLOG_DEBUG << "PRE PUSH";
                renderables.push_back(std::move(renderable));
                PLOG_DEBUG << "POST PUSH";
            }

            inline void Remove(std::shared_ptr<Renderable> renderable)
            {
                auto iter = std::find_if(renderables.begin(), renderables.end(),
                    [&](auto &r){ return (r->id == renderable->id); }
                );
                if (iter != renderables.end())
                {
                    renderables.erase(iter);
                }
            }
            
            inline std::shared_ptr<Renderable> FindObject(std::string objectName)
            {
                for(auto renderable : renderables)
                {
                    if(renderable->name == objectName)
                        return renderable;
                }
            }

            inline void RemoveObjectById(int id)
            {
                /*
                int index = 0;
                for(auto renderable : renderables)
                {
                    if(renderable->id == id)
                    {
                        renderables.erase(renderables.begin()+index);
                    }

                    index++;
                } 
                 */
            }
        };
    }
}
