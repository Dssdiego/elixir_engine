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

#endif //VULKAN_ENGINE_VULKANCOMMON_H
