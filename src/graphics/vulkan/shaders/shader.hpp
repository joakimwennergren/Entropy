#pragma once

#include <fstream>
#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>
#include <vulkan/vulkan.hpp>

namespace Entropy::Graphics::Vulkan::Shaders {
 class Shader {
 public:
  /**
  * @brief Constructs a Shader object from given vertex and fragment shader files.
  *
  * This constructor initializes the Shader object by reading and compiling
  * the provided vertex and fragment shader files.
  *
  * @param vert Path to the vertex shader file.
  * @param frag Path to the fragment shader file.
  * @return No return value as it is a constructor.
  */
  Shader(const std::string &vert, const std::string &frag);

  /**
   * @brief Destructor for the Shader class.
   *
   * This destructor releases resources associated with the Shader object,
   * specifically destroying the vertex and fragment shader modules.
   */
  ~Shader();

  [[nodiscard]] VkShaderModule GetVertShaderModule() const { return _shaderModuleVert; }

  [[nodiscard]] VkShaderModule GetFragShaderModule() const { return _shaderModuleFrag; }

 private:
  /**
  * @brief Reads the contents of a file and returns it as a vector of characters.
  *
  * This method opens the specified file in binary mode, reads its contents, and
  * returns the data as a vector of characters. If the file cannot be opened, it
  * logs an error message and terminates the application.
  *
  * @param filename The path to the file to be read.
  * @return A vector of characters containing the contents of the file.
  */
  static std::vector<char> ReadFile(std::string filename);

  /**
   * @brief Creates a Vulkan shader module from given shader code.
   *
   * This method initializes a Vulkan shader module using the provided
   * vector of characters containing shader code.
   *
   * @param code A vector of characters representing the shader code.
   * @return A VkShaderModule object representing the created shader module.
   */
  VkShaderModule BuildShader(const std::vector<char> &code) const;

  /**
   * @brief Constructs a Vulkan Shader Module from the provided shader code.
   *
   * This method generates a Vulkan Shader Module by creating and
   * initializing a VkShaderModuleCreateInfo structure with the given
   * shader code and its size, and then creating the module using Vulkan's
   * vkCreateShaderModule API.
   *
   * @param code Pointer to the buffer containing the shader code.
   * @param size Size in bytes of the shader code.
   * @return A VkShaderModule handle representing the created shader module.
   */
  VkShaderModule BuildShader(const uint32_t *code, uint32_t size) const;

 private:
  std::vector<char> _vertCode;
  std::vector<char> _fragCode;
  VkShaderModule _shaderModuleVert = VK_NULL_HANDLE;
  VkShaderModule _shaderModuleFrag = VK_NULL_HANDLE;
  std::shared_ptr<ILogicalDevice> _logicalDevice;
 };
} // namespace Entropy::Graphics::Vulkan::Shaders
