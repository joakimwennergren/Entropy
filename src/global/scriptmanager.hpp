#pragma once

#include <string>
#include <mutex>

#include <sol/sol.hpp>

// Primitives
#include <graphics/primitives/2d/sprite.hpp>

namespace Entropy
{
    namespace Global
    {
        class ScriptManager
        {
        private:
            static ScriptManager *pinstance_;
            static std::mutex mutex_;

            void Bind();

        protected:
            ScriptManager()
            {
                // by default, libraries are not opened
                // you can open libraries by using open_libraries
                // the libraries reside in the sol::lib enum class
                lua.open_libraries(sol::lib::base);

                // call lua code directly
                lua.script("print('hello world')");
            }

            ~ScriptManager() {}

            sol::state lua;

        public:
            ScriptManager(ScriptManager &other) = delete;
            void operator=(const ScriptManager &) = delete;
            static ScriptManager *GetInstance();
        };
    }
}
