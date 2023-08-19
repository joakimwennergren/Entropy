#pragma once

#include <fstream>
#include <vulkan/vulkan.hpp>

#include "context.hpp"

namespace Symbios
{
    namespace Graphics
    {
        namespace Shader
        {
            class Default
            {
            public:
                Default() = default;
                Default(const std::string vert, const std::string frag, std::shared_ptr<Symbios::Core::Context> context);

                void BuildShader(std::vector<char> code);

                // Getters
                std::vector<char> GetVertCode() { return this->_vertCode; };
                std::vector<char> GetFragCode() { return this->_fragCode; };

            private:
                std::vector<char> ReadFile(std::string filename);

            private:
                std::vector<char> _vertCode;
                std::vector<char> _fragCode;
                std::shared_ptr<Symbios::Core::Context> _context;
            };
        }
    }
}