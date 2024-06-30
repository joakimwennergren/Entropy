#pragma once

#include <string>
#include <sol/sol.hpp>


namespace Entropy
{
    namespace Scripting
    {
        class Script
        {
        public:
            Script() = default;
            std::string script;
            std::string scriptFile;
            sol::environment environment;
            bool hasExecuted = false;

            

        private:
        };
    }
}
