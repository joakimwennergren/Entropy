#include "pipeline.hpp"

using namespace Entropy::Graphics::Pipelines;

Pipeline::Pipeline(std::shared_ptr<RenderPass> renderPass, std::shared_ptr<ServiceLocator> serviceLocator)
{
    // Get required depenencies
    auto logicalDevice = std::dynamic_pointer_cast<LogicalDevice>(serviceLocator->getService("LogicalDevice"));
    auto swapChain = std::dynamic_pointer_cast<Swapchain>(serviceLocator->getService("SwapChain"));
    auto descriptorSetLayout = std::dynamic_pointer_cast<DescriptorsetLayout>(serviceLocator->getService("DescriptorSetLayout"));

    if (!logicalDevice->isValid())
    {
        spdlog::error("Trying to create pipeline with invalid invalid logical device");
        return;
    }

    if (!swapChain->isValid())
    {
        spdlog::error("Trying to create pipeline with invalid invalid swapchain");
        return;
    }

    if (!descriptorSetLayout->isValid())
    {
        spdlog::error("Trying to create pipeline with invalid descriptorset");
        return;
    }

    // Assign services
    _logicalDevice = logicalDevice;
    _swapchain = swapChain;
    _descriptorsetLayout = descriptorSetLayout;
    _serviceLocator = serviceLocator;

    auto shader = std::make_unique<Shader>(_serviceLocator, GetShadersDir() + "vert.spv", GetShadersDir() + "frag.spv");

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = shader->GetVertShaderModule();
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = shader->GetFragShaderModule();
    fragShaderStageInfo.pName = "main";

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages = {vertShaderStageInfo, fragShaderStageInfo};

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR};

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    auto bindingDescriptionVertex = Vertex::getBindingDescription();
    auto attributeDescriptionsVertex = Vertex::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptionsVertex.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescriptionVertex;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptionsVertex.data();

    std::vector<VkPipelineVertexInputStateCreateInfo> vertexInputStates(1);
    vertexInputStates[0] = vertexInputInfo;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)_swapchain->swapChainExtent.width;
    viewport.height = (float)_swapchain->swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = _swapchain->swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f;          // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f;    // Optional

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;          // Optional
    multisampling.pSampleMask = nullptr;            // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE;      // Optional

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    std::vector<VkDescriptorSetLayout> dsLayouts(2);

    VkDescriptorSetLayout tempLayout;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding texturesLayoutBinding{};
    texturesLayoutBinding.binding = 2;
    texturesLayoutBinding.descriptorCount = 1;
    texturesLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    texturesLayoutBinding.pImmutableSamplers = nullptr;
    texturesLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {texturesLayoutBinding, samplerLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(_logicalDevice->Get(), &layoutInfo, nullptr, &tempLayout) != VK_SUCCESS)
    {
        spdlog::error("Failed to create descriptor set layout!");
        return;
    }

    dsLayouts[0] = _descriptorsetLayout->Get();
    dsLayouts[1] = tempLayout;

    // setup push constants
    VkPushConstantRange push_constant;
    // this push constant range starts at the beginning
    push_constant.offset = 0;
    // this push constant range takes up the size of a MeshPushConstants struct
    push_constant.size = sizeof(PushConstant);
    // this push constant range is accessible only in the vertex shader
    push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = dsLayouts.size();
    pipelineLayoutInfo.pSetLayouts = dsLayouts.data();
    pipelineLayoutInfo.pPushConstantRanges = &push_constant;
    pipelineLayoutInfo.pushConstantRangeCount = 1;

    if (vkCreatePipelineLayout(_logicalDevice->Get(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
    {
        spdlog::error("Failed to create pipeline layout!");
        return;
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = vertexInputStates.data();
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = _pipelineLayout;
    pipelineInfo.renderPass = renderPass->GetRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(logicalDevice->Get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS)
    {
        spdlog::error("Failed to create pipeline!");
        return;
    }
}

Pipeline::Pipeline(std::shared_ptr<RenderPass> renderPass, uint32_t *vertContent, uint32_t vertSize, uint32_t *fragContent, uint32_t fragSize)
{
    /*
    _vkContext = VulkanContext::GetInstance();

#if defined(BUILD_FOR_ANDROID)
    _shader = std::make_unique<Shader>(vertContent, vertSize, fragContent, fragSize);
#endif

    // Create Shader and store it
#ifdef BUILD_FOR_IOS
    _shader = std::make_unique<Shader>(GetProjectBasePath() + "/vert.spv", GetProjectBasePath() + "/frag.spv");
#else
    auto shaderStages = CreateShaderStages();
#endif

    auto dynamicState = CreateDynamicState();

    auto inputAssembly = CreateInputAssembly();

    auto vertexInputStates = CreateVertexInputStates();

    auto viewportState = CreateViewportState();

    auto rasterizer = CreateRasterizer();

    auto multisampling = CreateMultisampling();

    auto colorBlending = CreateColorBlendning();

    auto dsLayouts = CreateDescriptorSetLayouts();

    auto push_constant = CreatePushContantRange();

    CreatePipelineLayout(dsLayouts, push_constant);

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = vertexInputStates.data();
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = this->_pipelineLayout;
    pipelineInfo.renderPass = renderPass->GetRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(_vkContext->logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->_pipeline) != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }
    */
}

Pipeline::~Pipeline()
{
    if (!_logicalDevice->isValid())
    {
        spdlog::error("Couldn't clean up after pipeline since logicaldevice is invalid");
        return;
    }

    vkDestroyDescriptorSetLayout(_logicalDevice->Get(), _descriptorSetLayout, nullptr);
    vkDestroyPipeline(_logicalDevice->Get(), _pipeline, nullptr);
    vkDestroyPipelineLayout(_logicalDevice->Get(), _pipelineLayout, nullptr);
}
