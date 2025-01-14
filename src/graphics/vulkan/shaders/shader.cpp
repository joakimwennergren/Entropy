#include "shader.hpp"

using namespace Entropy::Graphics::Vulkan::Shaders;

Shader::Shader(const std::string &vert, const std::string &frag) {
  _logicalDevice = ServiceLocator::GetInstance()->getService<ILogicalDevice>();
  _vertCode = ReadFile(vert);
  _fragCode = ReadFile(frag);
  if (!_vertCode.empty() && !_fragCode.empty()) {
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
    spdlog::error("Could not read shader: {}", filename);
    exit(EXIT_FAILURE);
  }

  const std::streamsize fileSize = file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

VkShaderModule Shader::BuildShader(const std::vector<char> &code) const {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  VkShaderModule shaderModule;
  VK_CHECK(vkCreateShaderModule(_logicalDevice->Get(), &createInfo, nullptr,
    &shaderModule));

  return shaderModule;
}

VkShaderModule Shader::BuildShader(const uint32_t *code, const uint32_t size) const {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = size;
  createInfo.pCode = code;

  VkShaderModule shaderModule;

  VK_CHECK(vkCreateShaderModule(_logicalDevice->Get(), &createInfo, nullptr,
    &shaderModule));

  return shaderModule;
}
