#include "shader.hpp"

using namespace Entropy::Graphics::Vulkan::Shaders;

Shader::Shader(const std::string vert, const std::string frag) {
  ServiceLocator *sl = ServiceLocator::GetInstance();
  _logicalDevice = sl->getService<ILogicalDevice>();

  _vertCode = this->ReadFile(vert);
  _fragCode = this->ReadFile(frag);

  if (this->_vertCode.size() > 0 && _fragCode.size() > 0) {
    _shaderModuleVert = BuildShader(_vertCode);
    _shaderModuleFrag = BuildShader(_fragCode);
  }
}

Shader::~Shader() {
  vkDestroyShaderModule(_logicalDevice->Get(), _shaderModuleVert, nullptr);
  vkDestroyShaderModule(_logicalDevice->Get(), _shaderModuleFrag, nullptr);
}

std::vector<char> Shader::ReadFile(std::string filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    spdlog::error("Couln't read shader: {}", filename);
    exit(EXIT_FAILURE);
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

VkShaderModule Shader::BuildShader(std::vector<char> code) {

  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  VkShaderModule shaderModule;
  VK_CHECK(vkCreateShaderModule(_logicalDevice->Get(), &createInfo, nullptr,
                                &shaderModule));

  return shaderModule;
}

VkShaderModule Shader::BuildShader(uint32_t *code, uint32_t size) {

  ServiceLocator *sl = ServiceLocator::GetInstance();
  auto logicalDevice = sl->getService<ILogicalDevice>();

  assert(logicalDevice != nullptr);

  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = size;
  createInfo.pCode = code;

  VkShaderModule shaderModule;

  if (vkCreateShaderModule(_logicalDevice->Get(), &createInfo, nullptr,
                           &shaderModule) != VK_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  return shaderModule;
}
