//
// Created by Diego S. Seabra on 07/09/22.
//

#ifndef VULKAN_ENGINE_VULKANDESCRIPTORSETLAYOUT_H
#define VULKAN_ENGINE_VULKANDESCRIPTORSETLAYOUT_H

#include <vulkan/vulkan.h>
#include <unordered_map>
#include <iostream>
#include <cassert>

class VulkanDescriptorSetLayout
{
public:
    VulkanDescriptorSetLayout(const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> &bindings);
    ~VulkanDescriptorSetLayout();

//    VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout &) = delete;
//    VulkanDescriptorSetLayout &operator=(const VulkanDescriptorSetLayout &) = delete;

    VkDescriptorSetLayout GetDescriptorSetLayout() const { return descriptorSetLayout; }

    VulkanDescriptorSetLayout &AddBinding(
            uint32_t binding,
            VkDescriptorType descriptorType,
            VkShaderStageFlags stageFlags,
            uint32_t count = 1
    );

    std::unique_ptr<VulkanDescriptorSetLayout> Build() const;

private:
    VkDescriptorSetLayout descriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
};


#endif //VULKAN_ENGINE_VULKANDESCRIPTORSETLAYOUT_H
