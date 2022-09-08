//
// Created by Diego S. Seabra on 07/09/22.
//

#ifndef VULKAN_ENGINE_VULKANDESCRIPTORWRITER_H
#define VULKAN_ENGINE_VULKANDESCRIPTORWRITER_H

#include "VulkanDescriptorSetLayout.h"
#include "VulkanDescriptorPool.h"

class VulkanDescriptorWriter
{
public:
    VulkanDescriptorWriter(VulkanDescriptorSetLayout &setLayout, VulkanDescriptorPool &pool);

    VulkanDescriptorWriter &WriteBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
    VulkanDescriptorWriter &WriteImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

    bool Build(VkDescriptorSet &set);
    void Overwrite(VkDescriptorSet &set);

private:
    VulkanDescriptorSetLayout &setLayout;
    VulkanDescriptorPool &pool;
    std::vector<VkWriteDescriptorSet> writes;
};


#endif //VULKAN_ENGINE_VULKANDESCRIPTORWRITER_H
