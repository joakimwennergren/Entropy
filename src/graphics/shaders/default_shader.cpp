#include "default_shader.hpp"

using namespace Symbios::Graphics::Shader;

Default::Default(const std::string vert, const std::string frag)
{
    this->_vertCode = this->ReadFile(vert);
    this->_fragCode = this->ReadFile(frag);
}

std::vector<char> Default::ReadFile(std::string filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

void Default::BuildShaders()
{
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = this->_vertCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(this->_fragCode.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create shader module!");
        }
    }
}