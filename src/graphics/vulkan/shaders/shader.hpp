#ifndef ENTROPY_SHADER_H
#define ENTROPY_SHADER_H

#include <fstream>
#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/utilities/helpers.hpp>
#include <vulkan/vulkan.hpp>

namespace Entropy::Graphics::Vulkan::Shaders {
    struct Shader {
        /**
         * @brief Constructor for the Shader class.
         *
         * This constructor initializes a Shader object by reading the vertex and fragment
         * shader source files and building their respective shader modules.
         *
         * @param vert The file path to the vertex shader source code.
         * @param frag The file path to the fragment shader source code.
         * @return None.
         */
        Shader(const std::string &vert, const std::string &frag);


        /**
         * @brief Destructor for the Shader class.
         *
         * Cleans up resources associated with the Shader object, specifically the
         * vertex and fragment shader modules. These Vulkan shader modules are
         * destroyed to release the allocated memory.
         *
         * @return None.
         */
        ~Shader();

        /**
         * @brief Retrieves the Vulkan shader module for the vertex shader.
         *
         * This method returns the Vulkan shader module associated with
         * the vertex shader.
         *
         * @return The Vulkan shader module for the vertex shader.
         */
        [[nodiscard]] VkShaderModule GetVertShaderModule() const { return _shaderModuleVert; }

        /**
         * @brief Retrieves the fragment shader module.
         *
         * This method returns the Vulkan shader module associated with the
         * fragment shader.
         *
         * @return The fragment shader module as a VkShaderModule.
         */
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

        std::vector<char> _vertCode;
        std::vector<char> _fragCode;
        VkShaderModule _shaderModuleVert = VK_NULL_HANDLE;
        VkShaderModule _shaderModuleFrag = VK_NULL_HANDLE;
        std::shared_ptr<ILogicalDevice> _logicalDevice;
    };
} // namespace Entropy::Graphics::Vulkan::Shaders

#endif // ENTROPY_SHADER_H
