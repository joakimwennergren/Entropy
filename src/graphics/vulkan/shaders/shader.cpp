#include "shader.hpp"

using namespace Symbios::Graphics::Vulkan::Shaders;

/*

Shader::Shader(std::shared_ptr<ServiceLocator> serviceLocator,
               const std::vector<char> vert, const std::vector<char> frag) {
  _logicalDevice = serviceLocator->GetService<LogicalDevice>();

  if (vert.size() > 0 && frag.size() > 0) {
    this->_shaderModuleVert = this->BuildShader(vert);
    this->_shaderModuleFrag = this->BuildShader(frag);
  }
}

*/

Shader::Shader(
    const std::string vert, const std::string frag)
{

  this->_vertCode = this->ReadFile(vert);
  this->_fragCode = this->ReadFile(frag);

  if (this->_vertCode.size() > 0 && this->_fragCode.size() > 0)
  {
    // this->_shaderModuleVert = this->BuildShader(backend, this->_vertCode);
    // this->_shaderModuleFrag = this->BuildShader(backend, this->_fragCode);
  }
}

Shader::~Shader()
{
  // vkDestroyShaderModule(_logicalDevice->Get(), this->_shaderModuleVert,
  // nullptr); vkDestroyShaderModule(_logicalDevice->Get(),
  // this->_shaderModuleFrag, nullptr);
}

std::vector<char> Shader::ReadFile(std::string filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open())
  {
    exit(EXIT_FAILURE);
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

VkShaderModule
Shader::BuildShader(
    std::vector<char> code)
{
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  VkShaderModule shaderModule;

  // if (vkCreateShaderModule(backend.logicalDevice.Get(), &createInfo, nullptr,
  //                          &shaderModule) != VK_SUCCESS)
  // {
  //   exit(EXIT_FAILURE);
  // }

  return shaderModule;
}

VkShaderModule Shader::BuildShader(uint32_t *code, uint32_t size)
{
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = size;
  createInfo.pCode = code;

  VkShaderModule shaderModule;

  if (vkCreateShaderModule(_logicalDevice->Get(), &createInfo, nullptr,
                           &shaderModule) != VK_SUCCESS)
  {
    exit(EXIT_FAILURE);
  }

  return shaderModule;
}
