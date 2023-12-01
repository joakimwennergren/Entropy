#include "shader.hpp"

using namespace Symbios::Graphics::Shaders;

Shader::Shader(const std::string vert, const std::string frag)
{
    this->_vertCode = this->ReadFile(vert);
    this->_fragCode = this->ReadFile(frag);

    if (this->_vertCode.size() > 0 && this->_fragCode.size() > 0)
    {
        this->_shaderModuleVert = this->BuildShader(this->_vertCode);
        this->_shaderModuleFrag = this->BuildShader(this->_fragCode);
    }
}

Shader::Shader(char *vertContent, uint32_t vertSize, char *fragContent, uint32_t fragSize)
{
    this->_shaderModuleVert = this->BuildShader(vertContent, vertSize);
    this->_shaderModuleFrag = this->BuildShader(fragContent, fragSize);
}

Shader::~Shader()
{
    VulkanContext *vkContext = VulkanContext::GetInstance();

    vkDestroyShaderModule(vkContext->logicalDevice, this->_shaderModuleVert, nullptr);
    vkDestroyShaderModule(vkContext->logicalDevice, this->_shaderModuleFrag, nullptr);
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

VkShaderModule Shader::BuildShader(std::vector<char> code)
{
    VulkanContext *vkContext = VulkanContext::GetInstance();

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkShaderModule shaderModule;

    if (vkCreateShaderModule(vkContext->logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }

    return shaderModule;
}

VkShaderModule Shader::BuildShader(char *code, uint32_t size)
{
    VulkanContext *vkContext = VulkanContext::GetInstance();

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = size;
    createInfo.pCode = (const uint32_t *)code;

    VkShaderModule shaderModule;

    if (vkCreateShaderModule(vkContext->logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }

    return shaderModule;
}
