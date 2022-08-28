//
// Created by Diego S. Seabra on 15/07/22.
//

#ifndef VULKAN_ENGINE_VULKANCOMMON_H
#define VULKAN_ENGINE_VULKANCOMMON_H

#define VK_CHECK(x)                                                 \
	do                                                              \
	{                                                               \
		VkResult err = x;                                           \
		if (err)                                                    \
		{                                                                 \
			std::cout << "Detected Vulkan error: " << err << std::endl; \
			abort();                                                \
		}                                                           \
	} while (0)

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

// The combination of a pipeline and it's layout
struct PipelineSet
{
//    std::string vertexShaderFilePath;
//    std::string fragmentShaderFilePath;
    std::string name;
    VkPipeline &pipeline;
    VkPipelineLayout &layout;
    VkPrimitiveTopology topology;
    VkPolygonMode polygonMode;
    VkBool32 enableBlending;
};

#endif //VULKAN_ENGINE_VULKANCOMMON_H
