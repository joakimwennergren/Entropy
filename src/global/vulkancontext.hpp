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

#include "context.hpp"

#include <chaiscript/chaiscript.hpp>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)

#include <GLFW/glfw3.h>

#endif

using namespace Symbios::Core;

namespace Symbios
{
        namespace Global
        {
            /**
             * The Singleton class defines the `GetInstance` method that serves as an
             * alternative to constructor and lets clients access the same instance of this
             * class over and over.
             */
            class VulkanContext
            {

                /**
                 * The Singleton's constructor/destructor should always be private to
                 * prevent direct construction/desctruction calls with the `new`/`delete`
                 * operator.
                 */
            private:
                static VulkanContext *pinstance_;
                static std::mutex mutex_;

            protected:
                VulkanContext()
                {
                    chai = new chaiscript::ChaiScript();
                    //_sceneGraph = std::make_shared<SceneGraph>();
                }

                ~VulkanContext() {}

                std::shared_ptr<Context> vulkanContext;
                chaiscript::ChaiScript *chai;

            public:
                /**
                 * Singletons should not be cloneable.
                 */
                VulkanContext(VulkanContext &other) = delete;
                /**
                 * Singletons should not be assignable.
                 */
                void operator=(const VulkanContext &) = delete;
                /**
                 * This is the static method that controls the access to the singleton
                 * instance. On the first run, it creates a singleton object and places it
                 * into the static field. On subsequent runs, it returns the client existing
                 * object stored in the static field.
                 */

                inline chaiscript::ChaiScript *GetChaiInstance() {return this->chai;};

                //inline std::shared_ptr<SceneGraph> GetSceneGraph() {return this->_sceneGraph;};

                static VulkanContext *GetInstance();
                /**
                 * Finally, any singleton should define some business logic, which can be
                 * executed on its instance.
                 */

                std::shared_ptr<Context> GetVulkanContext() { return this->vulkanContext;};

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)
                void InitializeContext(GLFWwindow * window)
                {
                    vulkanContext = std::make_shared<Context>(window);
                }
#else 
                void InitializeContext(CA::MetalLayer *layer, CGRect frame)
                {
                    vulkanContext = std::make_shared<Context>(layer, frame);
                }             
#endif

            };
        }
}
