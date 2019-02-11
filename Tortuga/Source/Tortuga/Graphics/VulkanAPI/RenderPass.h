#ifndef _VULKAN_API_RENDER_PASS
#define _VULKAN_API_RENDER_PASS

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"
#include "Swapchain.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class RenderPass
{
private:
  Device *_device;
  VkRenderPass _renderPass;

  static VkFormat FindDepthFormat(Device *device);
  static VkFormat FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features, Device *device);

public:
  RenderPass(Device *device, Swapchain *swapchain);
  ~RenderPass();

  VkRenderPass GetRenderPass() { return _renderPass; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif