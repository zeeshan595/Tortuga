#include "./Image.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Image
{
Image Create(Device::Device device, uint32_t width, uint32_t height)
{
  Image data = {};
  data.Device = device.Device;
  data.Width = width;
  data.Height = height;

  //Image handle
  VkImageCreateInfo imageInfo = {};
  {
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext = 0;
    imageInfo.flags = 0;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    imageInfo.extent = {width, height, 1};
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  }
  ErrorCheck::Callback(vkCreateImage(device.Device, &imageInfo, nullptr, &data.Image));

  //Memory
  VkMemoryRequirements memoryRequirements = {};
  vkGetImageMemoryRequirements(data.Device, data.Image, &memoryRequirements);

  VkMemoryAllocateInfo allocInfo = {};
  {
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = 0;
    allocInfo.memoryTypeIndex = Device::FindMemoryType(device, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    allocInfo.allocationSize = memoryRequirements.size;
  }
  ErrorCheck::Callback(vkAllocateMemory(data.Device, &allocInfo, nullptr, &data.Memory));
  ErrorCheck::Callback(vkBindImageMemory(device.Device, data.Image, data.Memory, 0));

  return data;
}

void Destroy(Image data)
{
  vkDestroyImage(data.Device, data.Image, nullptr);
  vkFreeMemory(data.Device, data.Memory, nullptr);
}
} // namespace Image
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga