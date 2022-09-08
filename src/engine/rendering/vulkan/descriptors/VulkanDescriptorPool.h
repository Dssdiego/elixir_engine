//
// Created by Diego S. Seabra on 07/09/22.
//

#ifndef VULKAN_ENGINE_VULKANDESCRIPTORPOOL_H
#define VULKAN_ENGINE_VULKANDESCRIPTORPOOL_H

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class VulkanDescriptorPool
{
public:
    VulkanDescriptorPool(
        uint32_t maxSets,
        VkDescriptorPoolCreateFlags poolFlags,
        const std::vector<VkDescriptorPoolSize> &poolSizes);
    ~VulkanDescriptorPool();

//    VulkanDescriptorPool(const VulkanDescriptorPool &) = delete;
//    VulkanDescriptorPool &operator=(const VulkanDescriptorPool &) = delete;

    VulkanDescriptorPool &AddPoolSize(VkDescriptorType descriptorType, uint32_t count);
    VulkanDescriptorPool &SetPoolFlags(VkDescriptorPoolCreateFlags flags);
    VulkanDescriptorPool &SetMaxSets(uint32_t count);
    std::unique_ptr<VulkanDescriptorPool> Build() const;

    bool AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;
    void FreeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

    void ResetPool();

private:
    std::vector<VkDescriptorPoolSize> poolSizes{};
    uint32_t maxSets = 1000;
    VkDescriptorPoolCreateFlags poolFlags = 0;

    VkDescriptorPool descriptorPool;
};


#endif //VULKAN_ENGINE_VULKANDESCRIPTORPOOL_H
