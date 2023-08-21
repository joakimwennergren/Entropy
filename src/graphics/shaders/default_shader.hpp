#pragma once

#include <fstream>
#include <vulkan/vulkan.hpp>
#include <plog/Log.h>

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
                Default(const std::string vert, const std::string frag, Symbios::Core::Context *context);

                ~Default();

                // Getters
                std::vector<char> GetVertCode() { return this->_vertCode; };
                std::vector<char> GetFragCode() { return this->_fragCode; };

                VkShaderModule shaderModuleVert;
                VkShaderModule shaderModuleFrag;

            private:
                std::vector<char> ReadFile(std::string filename);
                VkShaderModule BuildShader(std::vector<char> code);

            private:
                std::vector<char> _vertCode;
                std::vector<char> _fragCode;
                Symbios::Core::Context* _context;
            };
        }
    }
}