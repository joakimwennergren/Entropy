#include "model.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

using namespace Entropy::GLTF;

Model::Model(std::shared_ptr<ServiceLocator> serviceLocator)
{
    _serviceLocator = serviceLocator;
    type = 4;
    textureId = -1;
}

void Model::destroy(VkDevice device)
{
    if (vertices.buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(device, vertices.buffer, nullptr);
        vkFreeMemory(device, vertices.memory, nullptr);
        vertices.buffer = VK_NULL_HANDLE;
    }
    if (indices.buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(device, indices.buffer, nullptr);
        vkFreeMemory(device, indices.memory, nullptr);
        indices.buffer = VK_NULL_HANDLE;
    }

    /*
    for (auto texture : textures)
    {
        texture->reset();
    }
    */

    // textures.resize(0);
    textureSamplers.resize(0);
    for (auto node : nodes)
    {
        delete node;
    }
    materials.resize(0);
    animations.resize(0);
    nodes.resize(0);
    linearNodes.resize(0);
    extensions.resize(0);
    skins.resize(0);
};

void Model::loadNode(const tinygltf::Node &inputNode, const tinygltf::Model &input, Node *parent, uint32_t nodeIndex, std::vector<uint32_t> &indexBuffer, std::vector<Vertex> &vertexBuffer)
{
    Node *node = new Node();
    node->parent = parent;
    node->matrix = glm::mat4(1.0f);
    node->index = nodeIndex;
    node->skin = inputNode.skin;

    // Get the local node matrix
    // It's either made up from translation, rotation, scale or a 4x4 matrix
    if (inputNode.translation.size() == 3)
    {
        node->translation = glm::make_vec3(inputNode.translation.data());
    }
    if (inputNode.rotation.size() == 4)
    {
        glm::quat q = glm::make_quat(inputNode.rotation.data());
        node->rotation = glm::mat4(q);
    }
    if (inputNode.scale.size() == 3)
    {
        node->scale = glm::make_vec3(inputNode.scale.data());
    }
    if (inputNode.matrix.size() == 16)
    {
        node->matrix = glm::make_mat4x4(inputNode.matrix.data());
    };

    // Load node's children
    if (inputNode.children.size() > 0)
    {
        for (size_t i = 0; i < inputNode.children.size(); i++)
        {
            loadNode(input.nodes[inputNode.children[i]], input, node, inputNode.children[i], indexBuffer, vertexBuffer);
        }
    }

    // If the node contains mesh data, we load vertices and indices from the buffers
    // In glTF this is done via accessors and buffer views
    if (inputNode.mesh > -1)
    {
        const tinygltf::Mesh mesh = input.meshes[inputNode.mesh];
        // Iterate through all primitives of this node's mesh
        for (size_t i = 0; i < mesh.primitives.size(); i++)
        {
            const tinygltf::Primitive &glTFPrimitive = mesh.primitives[i];
            uint32_t firstIndex = static_cast<uint32_t>(indexBuffer.size());
            uint32_t vertexStart = static_cast<uint32_t>(vertexBuffer.size());
            uint32_t indexCount = 0;
            bool hasSkin = false;
            // Vertices
            {
                const float *positionBuffer = nullptr;
                const float *normalsBuffer = nullptr;
                const float *texCoordsBuffer = nullptr;
                const uint16_t *jointIndicesBuffer = nullptr;
                const float *jointWeightsBuffer = nullptr;
                size_t vertexCount = 0;

                // Get buffer data for vertex normals
                if (glTFPrimitive.attributes.find("POSITION") != glTFPrimitive.attributes.end())
                {
                    const tinygltf::Accessor &accessor = input.accessors[glTFPrimitive.attributes.find("POSITION")->second];
                    const tinygltf::BufferView &view = input.bufferViews[accessor.bufferView];
                    positionBuffer = reinterpret_cast<const float *>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
                    vertexCount = accessor.count;
                }
                // Get buffer data for vertex normals
                if (glTFPrimitive.attributes.find("NORMAL") != glTFPrimitive.attributes.end())
                {
                    const tinygltf::Accessor &accessor = input.accessors[glTFPrimitive.attributes.find("NORMAL")->second];
                    const tinygltf::BufferView &view = input.bufferViews[accessor.bufferView];
                    normalsBuffer = reinterpret_cast<const float *>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
                }
                // Get buffer data for vertex texture coordinates
                // glTF supports multiple sets, we only load the first one
                if (glTFPrimitive.attributes.find("TEXCOORD_0") != glTFPrimitive.attributes.end())
                {
                    const tinygltf::Accessor &accessor = input.accessors[glTFPrimitive.attributes.find("TEXCOORD_0")->second];
                    const tinygltf::BufferView &view = input.bufferViews[accessor.bufferView];
                    texCoordsBuffer = reinterpret_cast<const float *>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
                }

                // POI: Get buffer data required for vertex skinning
                // Get vertex joint indices
                if (glTFPrimitive.attributes.find("JOINTS_0") != glTFPrimitive.attributes.end())
                {
                    const tinygltf::Accessor &accessor = input.accessors[glTFPrimitive.attributes.find("JOINTS_0")->second];
                    const tinygltf::BufferView &view = input.bufferViews[accessor.bufferView];
                    jointIndicesBuffer = reinterpret_cast<const uint16_t *>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
                }
                // Get vertex joint weights
                if (glTFPrimitive.attributes.find("WEIGHTS_0") != glTFPrimitive.attributes.end())
                {
                    const tinygltf::Accessor &accessor = input.accessors[glTFPrimitive.attributes.find("WEIGHTS_0")->second];
                    const tinygltf::BufferView &view = input.bufferViews[accessor.bufferView];
                    jointWeightsBuffer = reinterpret_cast<const float *>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
                }

                hasSkin = (jointIndicesBuffer && jointWeightsBuffer);

                // Append data to model's vertex buffer
                for (size_t v = 0; v < vertexCount; v++)
                {
                    Vertex vert{};
                    vert.position = glm::vec4(glm::make_vec3(&positionBuffer[v * 3]), 1.0f);
                    vert.normal = glm::normalize(glm::vec3(normalsBuffer ? glm::make_vec3(&normalsBuffer[v * 3]) : glm::vec3(0.0f)));
                    vert.uv0 = texCoordsBuffer ? glm::make_vec2(&texCoordsBuffer[v * 2]) : glm::vec3(0.0f);
                    vert.color = glm::vec4(1.0f);
                    vert.joint0 = hasSkin ? glm::vec4(glm::make_vec4(&jointIndicesBuffer[v * 4])) : glm::vec4(0.0f);
                    vert.weight0 = hasSkin ? glm::make_vec4(&jointWeightsBuffer[v * 4]) : glm::vec4(0.0f);
                    vertexBuffer.push_back(vert);
                }
            }
            // Indices
            {
                const tinygltf::Accessor &accessor = input.accessors[glTFPrimitive.indices];
                const tinygltf::BufferView &bufferView = input.bufferViews[accessor.bufferView];
                const tinygltf::Buffer &buffer = input.buffers[bufferView.buffer];

                indexCount += static_cast<uint32_t>(accessor.count);

                // glTF supports different component types of indices
                switch (accessor.componentType)
                {
                case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
                {
                    const uint32_t *buf = reinterpret_cast<const uint32_t *>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
                    for (size_t index = 0; index < accessor.count; index++)
                    {
                        indexBuffer.push_back(buf[index] + vertexStart);
                    }
                    break;
                }
                case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
                {
                    const uint16_t *buf = reinterpret_cast<const uint16_t *>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
                    for (size_t index = 0; index < accessor.count; index++)
                    {
                        indexBuffer.push_back(buf[index] + vertexStart);
                    }
                    break;
                }
                case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
                {
                    const uint8_t *buf = reinterpret_cast<const uint8_t *>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
                    for (size_t index = 0; index < accessor.count; index++)
                    {
                        indexBuffer.push_back(buf[index] + vertexStart);
                    }
                    break;
                }
                default:
                    std::cerr << "Index component type " << accessor.componentType << " not supported!" << std::endl;
                    return;
                }
            }
            Primitive primitive{};
            primitive.firstIndex = firstIndex;
            primitive.indexCount = indexCount;
            primitive.materialIndex = glTFPrimitive.material;
            node->mesh.primitives.push_back(primitive);
        }
    }

    if (parent)
    {
        parent->children.push_back(node);
    }
    else
    {
        nodes.push_back(node);
    }
}

// POI: Load the skins from the glTF model
void Model::loadSkins(tinygltf::Model &input)
{
    skins.resize(input.skins.size());

    for (size_t i = 0; i < input.skins.size(); i++)
    {
        tinygltf::Skin glTFSkin = input.skins[i];

        skins[i].name = glTFSkin.name;
        // Find the root node of the skeleton
        skins[i].skeletonRoot = nodeFromIndex(glTFSkin.skeleton);

        // Find joint nodes
        for (int jointIndex : glTFSkin.joints)
        {
            Node *node = nodeFromIndex(jointIndex);
            if (node)
            {
                skins[i].joints.push_back(node);
            }
        }

        // Get the inverse bind matrices from the buffer associated to this skin
        if (glTFSkin.inverseBindMatrices > -1)
        {
            const tinygltf::Accessor &accessor = input.accessors[glTFSkin.inverseBindMatrices];
            const tinygltf::BufferView &bufferView = input.bufferViews[accessor.bufferView];
            const tinygltf::Buffer &buffer = input.buffers[bufferView.buffer];
            skins[i].inverseBindMatrices.resize(accessor.count);
            memcpy(skins[i].inverseBindMatrices.data(), &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(glm::mat4));

            // Store inverse bind matrices for this skin in a shader storage buffer object
            // To keep this sample simple, we create a host visible shader storage buffer
            skins[i].ssbo = std::make_unique<StorageBuffer>(_serviceLocator, sizeof(glm::mat4) * skins[i].inverseBindMatrices.size(), skins[i].inverseBindMatrices.data());
        }
    }
}

void Model::loadTextures(tinygltf::Model &input)
{
    textures.resize(input.textures.size());
    for (size_t i = 0; i < input.textures.size(); i++)
    {
        textures[i]->imageIndex = input.textures[i].source;
    }
}

VkSamplerAddressMode Model::getVkWrapMode(int32_t wrapMode)
{
    switch (wrapMode)
    {
    case -1:
    case 10497:
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case 33071:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case 33648:
        return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    }

    std::cerr << "Unknown wrap mode for getVkWrapMode: " << wrapMode << std::endl;
    return VK_SAMPLER_ADDRESS_MODE_REPEAT;
}

VkFilter Model::getVkFilterMode(int32_t filterMode)
{
    switch (filterMode)
    {
    case -1:
    case 9728:
        return VK_FILTER_NEAREST;
    case 9729:
        return VK_FILTER_LINEAR;
    case 9984:
        return VK_FILTER_NEAREST;
    case 9985:
        return VK_FILTER_NEAREST;
    case 9986:
        return VK_FILTER_LINEAR;
    case 9987:
        return VK_FILTER_LINEAR;
    }

    std::cerr << "Unknown filter mode for getVkFilterMode: " << filterMode << std::endl;
    return VK_FILTER_NEAREST;
}

void Model::loadTextureSamplers(tinygltf::Model &gltfModel)
{
    for (tinygltf::Sampler smpl : gltfModel.samplers)
    {
        TextureSampler sampler{};
        sampler.minFilter = getVkFilterMode(smpl.minFilter);
        sampler.magFilter = getVkFilterMode(smpl.magFilter);
        sampler.addressModeU = getVkWrapMode(smpl.wrapS);
        sampler.addressModeV = getVkWrapMode(smpl.wrapT);
        sampler.addressModeW = sampler.addressModeV;
        textureSamplers.push_back(sampler);
    }
}

void Model::loadMaterials(tinygltf::Model &input)
{
    materials.resize(input.materials.size());
    for (size_t i = 0; i < input.materials.size(); i++)
    {
        // We only read the most basic properties required for our sample
        tinygltf::Material glTFMaterial = input.materials[i];
        // Get the base color factor
        if (glTFMaterial.values.find("baseColorFactor") != glTFMaterial.values.end())
        {
            materials[i].baseColorFactor = glm::make_vec4(glTFMaterial.values["baseColorFactor"].ColorFactor().data());
        }
        // Get base color texture index
        if (glTFMaterial.values.find("baseColorTexture") != glTFMaterial.values.end())
        {
            materials[i].baseColorTextureIndex = glTFMaterial.values["baseColorTexture"].TextureIndex();
        }
    }
}

void Model::loadAnimations(tinygltf::Model &input)
{
    animations.resize(input.animations.size());

    for (size_t i = 0; i < input.animations.size(); i++)
    {
        tinygltf::Animation glTFAnimation = input.animations[i];
        animations[i].name = glTFAnimation.name;

        // Samplers
        animations[i].samplers.resize(glTFAnimation.samplers.size());
        for (size_t j = 0; j < glTFAnimation.samplers.size(); j++)
        {
            tinygltf::AnimationSampler glTFSampler = glTFAnimation.samplers[j];
            AnimationSampler &dstSampler = animations[i].samplers[j];
            dstSampler.interpolation = dstSampler.interpolation;

            // Read sampler keyframe input time values
            {
                const tinygltf::Accessor &accessor = input.accessors[glTFSampler.input];
                const tinygltf::BufferView &bufferView = input.bufferViews[accessor.bufferView];
                const tinygltf::Buffer &buffer = input.buffers[bufferView.buffer];
                const void *dataPtr = &buffer.data[accessor.byteOffset + bufferView.byteOffset];
                const float *buf = static_cast<const float *>(dataPtr);
                for (size_t index = 0; index < accessor.count; index++)
                {
                    dstSampler.inputs.push_back(buf[index]);
                }
                // Adjust animation's start and end times
                for (auto input : animations[i].samplers[j].inputs)
                {
                    if (input < animations[i].start)
                    {
                        animations[i].start = input;
                    };
                    if (input > animations[i].end)
                    {
                        animations[i].end = input;
                    }
                }
            }

            // Read sampler keyframe output translate/rotate/scale values
            {
                const tinygltf::Accessor &accessor = input.accessors[glTFSampler.output];
                const tinygltf::BufferView &bufferView = input.bufferViews[accessor.bufferView];
                const tinygltf::Buffer &buffer = input.buffers[bufferView.buffer];
                const void *dataPtr = &buffer.data[accessor.byteOffset + bufferView.byteOffset];
                switch (accessor.type)
                {
                case TINYGLTF_TYPE_VEC3:
                {
                    const glm::vec3 *buf = static_cast<const glm::vec3 *>(dataPtr);
                    for (size_t index = 0; index < accessor.count; index++)
                    {
                        dstSampler.outputsVec4.push_back(glm::vec4(buf[index], 0.0f));
                    }
                    break;
                }
                case TINYGLTF_TYPE_VEC4:
                {
                    const glm::vec4 *buf = static_cast<const glm::vec4 *>(dataPtr);
                    for (size_t index = 0; index < accessor.count; index++)
                    {
                        dstSampler.outputsVec4.push_back(buf[index]);
                    }
                    break;
                }
                default:
                {
                    std::cout << "unknown type" << std::endl;
                    break;
                }
                }
            }
        }

        // Channels
        animations[i].channels.resize(glTFAnimation.channels.size());
        for (size_t j = 0; j < glTFAnimation.channels.size(); j++)
        {
            tinygltf::AnimationChannel glTFChannel = glTFAnimation.channels[j];
            AnimationChannel &dstChannel = animations[i].channels[j];
            dstChannel.path = glTFChannel.target_path;
            dstChannel.samplerIndex = glTFChannel.sampler;
            dstChannel.node = nodeFromIndex(glTFChannel.target_node);
        }
    }
}

void Model::LoadFromFile(std::string filename, float size = 1.0f)
{
    tinygltf::Model glTFInput;
    tinygltf::TinyGLTF gltfContext;
    std::string error, warning;

#if defined(__ANDROID__)
    // On Android all assets are packed with the apk in a compressed form, so we need to open them using the asset manager
    // We let tinygltf handle this, by passing the asset manager of our app
    tinygltf::asset_manager = androidApp->activity->assetManager;
#endif
    bool fileLoaded = gltfContext.LoadASCIIFromFile(&glTFInput, &error, &warning, filename);

    std::vector<uint32_t> indexBuffer;
    std::vector<Vertex> vertexBuffer;

    if (fileLoaded)
    {
        // loadImages(glTFInput);
        // loadMaterials(glTFInput);
        // loadTextures(glTFInput);
        const tinygltf::Scene &scene = glTFInput.scenes[0];
        for (size_t i = 0; i < scene.nodes.size(); i++)
        {
            const tinygltf::Node node = glTFInput.nodes[scene.nodes[i]];
            loadNode(node, glTFInput, nullptr, scene.nodes[i], indexBuffer, vertexBuffer);
        }
        // loadSkins(glTFInput);

        // Get required depenencies
        auto logicalDevice = std::dynamic_pointer_cast<LogicalDevice>(_serviceLocator->getService("LogicalDevice"));
        // Get required depenencies
        auto descriptorPool = std::dynamic_pointer_cast<DescriptorPool>(_serviceLocator->getService("DescriptorPool"));

        VkDescriptorSetLayoutBinding skinBinding{};
        skinBinding.binding = 0;
        skinBinding.descriptorCount = 1;
        skinBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        skinBinding.pImmutableSamplers = nullptr;
        skinBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        std::vector<VkDescriptorSetLayoutBinding> bindings = {skinBinding};
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        VkDescriptorSetLayout descriptorSetLayout;

        if (vkCreateDescriptorSetLayout(logicalDevice->Get(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create descriptor set layout!");
        }

        // Descriptor set for glTF model skin joint matrices
        for (auto &skin : skins)
        {
            std::vector<VkDescriptorSetLayout> layouts(MAX_CONCURRENT_FRAMES_IN_FLIGHT, descriptorSetLayout);

            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = descriptorPool->Get();
            allocInfo.descriptorSetCount = MAX_CONCURRENT_FRAMES_IN_FLIGHT;
            allocInfo.pSetLayouts = layouts.data();

            if (vkAllocateDescriptorSets(logicalDevice->Get(), &allocInfo, &skin.descriptorSet) != VK_SUCCESS)
            {
                exit(EXIT_FAILURE);
            }

            VkDescriptorBufferInfo descriptor;
            descriptor.offset = 0;
            descriptor.buffer = skin.ssbo->GetVulkanBuffer();
            descriptor.range = skin.joints.size() * sizeof(glm::mat4);

            std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = skin.descriptorSet;
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &descriptor;

            vkUpdateDescriptorSets(logicalDevice->Get(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }

        loadAnimations(glTFInput);
        // Calculate initial pose
        for (auto node : nodes)
        {
            updateJoints(node);
        }
    }
    else
    {
        exit(-1);
        return;
    }

    for (int i = 0; i < vertexBuffer.size(); i++)
    {
        vertices_temp.push_back(vertexBuffer[i]);
    }

    // Create staging buffers
    // Vertex data
    _vertexBuffer = std::make_unique<VertexBuffer>(_serviceLocator, vertices_temp);

    // Index data
    if (indexBuffer.size() > 0)
    {
        std::vector<uint32_t> indices;

        for (int i = 0; i < indexBuffer.size(); i++)
        {
            indices.push_back(indexBuffer[i]);
        }

        _indexBuffer = std::make_unique<Buffer>();
        _indexBuffer->CreateIndexBufferUint32(_serviceLocator, indices);
    }

    /*
    // Descriptor sets for glTF model materials
    for (auto &image : images)
    {
        const VkDescriptorSetAllocateInfo allocInfo = vks::initializers::descriptorSetAllocateInfo(descriptorPool, &descriptorSetLayouts.textures, 1);
        VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &image.descriptorSet));
        VkWriteDescriptorSet writeDescriptorSet = vks::initializers::writeDescriptorSet(image.descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &image.texture.descriptor);
        vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);
    }
    */
}

void Model::drawNode(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, Node *node)
{
    if (node->mesh.primitives.size() > 0)
    {
        // Pass the node's matrix via push constants
        // Traverse the node hierarchy to the top-most parent to get the final matrix of the current node
        glm::mat4 nodeMatrix = node->matrix;
        Node *currentParent = node->parent;

        while (currentParent)
        {
            nodeMatrix = currentParent->matrix * nodeMatrix;
            currentParent = currentParent->parent;
        }

        auto translate = glm::translate(glm::mat4(1.0f), position);
        auto scaling = glm::scale(glm::mat4(1.0f), this->scale);

        auto o = glm::vec3(0.0, 0.0, 0.0);
        if (orientation == 1)
        {
            o.x = 1;
        }

        if (orientation == 2)
        {
            o.y = 1;
        }

        if (orientation == 3)
        {
            o.z = 1;
        }

        auto modelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotationX), o);

        nodeMatrix *= translate * scaling * modelRotation;

        // Pass the final matrix to the vertex shader using push constants
        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &nodeMatrix);

        if (node->skin != -1)
        {
            // Bind SSBO with skin data for this node to set 1
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 2, 1, &skins[node->skin].descriptorSet, 0, nullptr);
        }

        for (Primitive &primitive : node->mesh.primitives)
        {
            if (primitive.indexCount > 0)
            {
                vkCmdDrawIndexed(commandBuffer, primitive.indexCount, 1, primitive.firstIndex, 0, 0);
            }
        }
    }
    for (auto child : node->children)
    {
        drawNode(commandBuffer, pipelineLayout, child);
    }
}

void Model::draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout)
{
    // All vertices and indices are stored in single buffers, so we only need to bind once
    VkDeviceSize offsets[1] = {0};
    auto vertices = _vertexBuffer->GetVulkanBuffer();
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertices, offsets);
    vkCmdBindIndexBuffer(commandBuffer, _indexBuffer->GetVulkanBuffer(), 0, VK_INDEX_TYPE_UINT32);
    // Render all nodes at top-level
    for (auto node : nodes)
    {
        drawNode(commandBuffer, pipelineLayout, node);
    }
}

void Model::updateAnimation(float deltaTime)
{
    if (activeAnimation > static_cast<uint32_t>(animations.size()) - 1)
    {
        std::cout << "No animation with index " << activeAnimation << std::endl;
        return;
    }
    Animation &animation = animations[activeAnimation];
    animation.currentTime += deltaTime;
    if (animation.currentTime > animation.end)
    {
        animation.currentTime -= animation.end;
    }

    for (auto &channel : animation.channels)
    {
        AnimationSampler &sampler = animation.samplers[channel.samplerIndex];
        for (size_t i = 0; i < sampler.inputs.size() - 1; i++)
        {
            /*
            if (sampler.interpolation != "LINEAR")
            {
                std::cout << "This sample only supports linear interpolations\n";
                continue;
            }
            */

            // Get the input keyframe values for the current time stamp
            if ((animation.currentTime >= sampler.inputs[i]) && (animation.currentTime <= sampler.inputs[i + 1]))
            {
                float a = (animation.currentTime - sampler.inputs[i]) / (sampler.inputs[i + 1] - sampler.inputs[i]);
                if (channel.path == "translation")
                {
                    channel.node->translation = glm::mix(sampler.outputsVec4[i], sampler.outputsVec4[i + 1], a);
                }
                if (channel.path == "rotation")
                {
                    glm::quat q1;
                    q1.x = sampler.outputsVec4[i].x;
                    q1.y = sampler.outputsVec4[i].y;
                    q1.z = sampler.outputsVec4[i].z;
                    q1.w = sampler.outputsVec4[i].w;

                    glm::quat q2;
                    q2.x = sampler.outputsVec4[i + 1].x;
                    q2.y = sampler.outputsVec4[i + 1].y;
                    q2.z = sampler.outputsVec4[i + 1].z;
                    q2.w = sampler.outputsVec4[i + 1].w;

                    channel.node->rotation = glm::normalize(glm::slerp(q1, q2, a));
                }
                if (channel.path == "scale")
                {
                    channel.node->scale = glm::mix(sampler.outputsVec4[i], sampler.outputsVec4[i + 1], a);
                }
            }
        }
    }
    for (auto &node : nodes)
    {
        updateJoints(node);
    }
}

Node *Model::findNode(Node *parent, uint32_t index)
{
    Node *nodeFound = nullptr;
    if (parent->index == index)
    {
        return parent;
    }
    for (auto &child : parent->children)
    {
        nodeFound = findNode(child, index);
        if (nodeFound)
        {
            break;
        }
    }
    return nodeFound;
}

Node *Model::nodeFromIndex(uint32_t index)
{
    Node *nodeFound = nullptr;
    for (auto &node : nodes)
    {
        nodeFound = findNode(node, index);
        if (nodeFound)
        {
            break;
        }
    }
    return nodeFound;
}

glm::mat4 Node::getLocalMatrix()
{
    return glm::translate(glm::mat4(1.0f), translation) * glm::mat4(rotation) * glm::scale(glm::mat4(1.0f), scale) * matrix;
}

void Model::updateJoints(Node *node)
{
    if (node->skin > -1)
    {
        // Update the joint matrices
        glm::mat4 inverseTransform = glm::inverse(node->getLocalMatrix());
        Skin &skin = skins[node->skin];
        size_t numJoints = (uint32_t)skin.joints.size();
        std::vector<glm::mat4> jointMatrices(numJoints);
        for (size_t i = 0; i < numJoints; i++)
        {
            jointMatrices[i] = skin.joints[i]->getLocalMatrix() * skin.inverseBindMatrices[i];
            jointMatrices[i] = inverseTransform * jointMatrices[i];
        }
        // Update ssbo
        memcpy(skin.ssbo->GetMappedMemory(), jointMatrices.data(), jointMatrices.size() * sizeof(glm::mat4));
    }

    for (auto &child : node->children)
    {
        updateJoints(child);
    }
}