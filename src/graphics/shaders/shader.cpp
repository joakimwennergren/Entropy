#include "shader.hpp"

using namespace Symbios::Graphics::Shaders;

Shader::Shader(const std::string vert, const std::string frag, Symbios::Core::Context *context)
{
    this->_context = context;

    this->_vertCode = this->ReadFile(vert);
    this->_fragCode = this->ReadFile(frag);

    if (this->_vertCode.size() > 0 && this->_fragCode.size() > 0)
    {
        this->shaderModuleVert = this->BuildShader(this->_vertCode);
        this->shaderModuleFrag = this->BuildShader(this->_fragCode);
    }
}

Shader::~Shader()
{
    vkDestroyShaderModule(this->_context->GetLogicalDevice(), this->shaderModuleVert, nullptr);
    vkDestroyShaderModule(this->_context->GetLogicalDevice(), this->shaderModuleFrag, nullptr);
}

std::vector<char> Shader::ReadFile(std::string filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        PLOG_FATAL << "Failed to open shader: " << filename;
        exit(EXIT_FAILURE);
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

VkShaderModule Shader::BuildShader(std::vector<char> code)
{

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkShaderModule shaderModule;

    if (vkCreateShaderModule(this->_context->GetLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        PLOG_FATAL << "failed to create shader module!";
        exit(EXIT_FAILURE);
    }

    return shaderModule;
}