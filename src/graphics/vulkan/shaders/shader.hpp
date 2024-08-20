#pragma once

#include <fstream>
#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>
#include <servicelocators/servicelocator.hpp>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Shaders {

class Shader {
public:
  /**
   * @brief Construct a new Shader object
   *
   * @param vert
   * @param frag
   * @param contexts
   */
  Shader(const std::string vert, const std::string frag);

  /**
   * @brief Destroy the Shader object
   *
   */
  ~Shader();

  /**
   * @brief Get the Vert Shader Module object
   *
   * @return VkShaderModule
   */
  inline VkShaderModule GetVertShaderModule() { return _shaderModuleVert; };

  /**
   * @brief Get the Frag Shader Module object
   *
   * @return VkShaderModule
   */
  inline VkShaderModule GetFragShaderModule() { return _shaderModuleFrag; };

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
  VkShaderModule _shaderModuleVert = VK_NULL_HANDLE;
  VkShaderModule _shaderModuleFrag = VK_NULL_HANDLE;
  std::shared_ptr<ILogicalDevice> _logicalDevice;
};
} // namespace Shaders
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
