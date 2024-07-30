#pragma once

#include <vulkan/vulkan.hpp>
#include <fstream>
#include <servicelocators/servicelocator.hpp>
#include <graphics/vulkan/devices/ilogical_device.hpp>

namespace Entropy
{
  namespace Graphics
  {
    namespace Vulkan
    {
      namespace Shaders
      {
        class Shader
        {
        public:
          /**
           * @brief Construct a new Shader object
           *
           * @param vert
           * @param frag
           * @param context
           */
          Shader(const std::string vert, const std::string frag);

          // Shader(std::shared_ptr<ServiceLocator> serviceLocator,
          //        const std::vector<char> vert, const std::vector<char> frag);

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
          inline VkShaderModule GetVertShaderModule()
          {
            return this->_shaderModuleVert;
          };

          /**
           * @brief Get the Frag Shader Module object
           *
           * @return VkShaderModule
           */
          inline VkShaderModule GetFragShaderModule()
          {
            return this->_shaderModuleFrag;
          };

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

          /**
           * @brief
           *
           * @param code
           * @return VkShaderModule
           */
          VkShaderModule BuildShader(uint32_t *code, uint32_t size);

        private:
          std::vector<char> _vertCode;
          std::vector<char> _fragCode;
          VkShaderModule _shaderModuleVert;
          VkShaderModule _shaderModuleFrag;
        };
      } // namespace Shaders
    } // namespace Vulkan
  } // namespace Graphics
} // namespace Symbios
