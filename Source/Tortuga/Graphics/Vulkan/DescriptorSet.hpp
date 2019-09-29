#ifndef _VULKAN_DESCRIPTOR_SET
#define _VULKAN_DESCRIPTOR_SET

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"
#include "./DescriptorLayout.hpp"
#include "./DescriptorPool.hpp"
#include "./Buffer.hpp"
#include "./Image.hpp"
#include "./ImageView.hpp"
#include "./Sampler.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace DescriptorSet
{
struct DescriptorSet
{
  VkDevice Device = VK_NULL_HANDLE;
  VkDescriptorSet set = VK_NULL_HANDLE;
  DescriptorPool::DescriptorPool Pool;
  DescriptorLayout::DescriptorLayout Layout;
};

DescriptorSet Create(Device::Device device, DescriptorPool::DescriptorPool pool, DescriptorLayout::DescriptorLayout layout);
void UpdateDescriptorSet(DescriptorSet data, std::vector<Buffer::Buffer> content);
void UpdateDescriptorSet(DescriptorSet data, std::vector<ImageView::ImageView> content, std::vector<Sampler::Sampler> samplers);
} // namespace DescriptorSet
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif