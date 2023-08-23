/**
 * @file shader.hpp
 * @author Joakim Wennergren (joakim.wennergren@databeams.se)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <fstream>

#include "context.hpp"

using namespace Symbios::Core;

namespace Symbios
{
    namespace Graphics
    {
        namespace Shaders
        {
            class Shader
            {
            public:
                /**
                 * @brief Construct a new Shader object
                 *
                 */
                Shader() = default;
                /**
                 * @brief Construct a new Shader object
                 *
                 * @param vert
                 * @param frag
                 * @param context
                 */
                Shader(std::shared_ptr<Context> context, const std::string vert, const std::string frag);

                /**
                 * @brief Destroy the Shader object
                 *
                 */
                ~Shader();

                /**
                 * @brief Get the Vert Code object
                 *
                 * @return std::vector<char>
                 */
                inline std::vector<char> GetVertCode() { return this->_vertCode; };

                /**
                 * @brief Get the Frag Code object
                 *
                 * @return std::vector<char>
                 */
                inline std::vector<char> GetFragCode() { return this->_fragCode; };

                /**
                 * @brief Get the Vert Shader Module object
                 *
                 * @return VkShaderModule
                 */
                inline VkShaderModule GetVertShaderModule() { return this->_shaderModuleVert; };

                /**
                 * @brief Get the Frag Shader Module object
                 *
                 * @return VkShaderModule
                 */
                inline VkShaderModule GetFragShaderModule() { return this->_shaderModuleFrag; };

            private:
                /**
                 * @brief
                 *
                 * @param filename
                 * @return std::vector<char>
                 */
                std::vector<char> ReadFile(std::string filename);

                /**
                 * @brief
                 *
                 * @param code
                 * @return VkShaderModule
                 */
                VkShaderModule BuildShader(std::vector<char> code);

            private:
                std::shared_ptr<Context> _context;
                std::vector<char> _vertCode;
                std::vector<char> _fragCode;
                VkShaderModule _shaderModuleVert;
                VkShaderModule _shaderModuleFrag;
            };
        }
    }
}