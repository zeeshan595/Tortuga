#ifndef _RENDERER
#define _RENDERER

#include "./Vulkan/ErrorCheck.hpp"
#include "../Utils/IO.hpp"
#include "./Vulkan/Instance.hpp"
#include "./Vulkan/Device.hpp"
#include "./Vulkan/DescriptorLayout.hpp"
#include "./Vulkan/DescriptorPool.hpp"
#include "./Vulkan/DescriptorSets.hpp"
#include "./Vulkan/Shader.hpp"
#include "./Vulkan/Pipeline.hpp"
#include "./Vulkan/Buffer.hpp"
#include "./Vulkan/CommandPool.hpp"
#include "./Vulkan/Command.hpp"
#include "./RenderImage.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Renderer
{
struct Renderer
{
    std::vector<VkQueue> Queues;
    Vulkan::CommandPool::CommandPool CommandPool;
    Vulkan::Command::Command Command;
    Vulkan::DescriptorLayout::DescriptorLayout DescriptorLayout;
    Vulkan::DescriptorPool::DescriptorPool DescriptorPool;
    Vulkan::DescriptorSets::DescriptorSets DescriptorSet;
    Vulkan::Shader::Shader Shader;
    Vulkan::Pipeline::Pipeline Pipeline;
    Vulkan::Buffer::Buffer StagingBuffer;
    Vulkan::Buffer::Buffer Buffer;
    RenderImage::RenderImage Image;
};

Renderer Create(Vulkan::Instance::Instance vulkan, Vulkan::Device::Device device, uint32_t renderWidth, uint32_t renderHeight, std::vector<Vulkan::DescriptorLayout::DescriptorLayout> layouts = {});
void Destroy(Renderer data);
void Render(Renderer &data);
void BindDescriptorSets(Renderer &data, std::vector<Vulkan::DescriptorSets::DescriptorSets> descriptorSets = {});
} // namespace Renderer
} // namespace Graphics
} // namespace Tortuga

#endif