#include "./Rendering.hpp"

namespace Tortuga
{
namespace Systems
{
void Rendering::Update()
{
  auto device = Core::Engine::GetMainDevice();
  auto swapchain = Core::Screen::GetSwapchain();

  Graphics::Vulkan::Command::Begin(Renderer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  Graphics::Vulkan::Command::BeginRenderPass(Renderer, RenderPass, Framebuffers[0], swapchain.Extent.width, swapchain.Extent.height);
  Graphics::Vulkan::Command::BindPipeline(Renderer, VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline, {});
  vkCmdDraw(Renderer.Command, 3, 1, 0, 0);
  Graphics::Vulkan::Command::EndRenderPass(Renderer);
  Graphics::Vulkan::Command::End(Renderer);
  Graphics::Vulkan::Command::Submit({Renderer}, device.Queues.Graphics[0]);
}
Rendering::Rendering()
{
  auto vulkan = Core::Engine::GetVulkan();
  auto device = Core::Engine::GetMainDevice();
  auto swapchain = Core::Screen::GetSwapchain();

  //general
  {
    TransferCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
    ComputeCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Compute.Index);
    GraphicsCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
  }

  //graphics pipeline
  {
    auto vertexCode = Utils::IO::GetFileContents("Shaders/simple.vert");
    auto vertexCompile = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::VERTEX, vertexCode);
    auto fragmentCode = Utils::IO::GetFileContents("Shaders/simple.vert");
    auto fragmentCompile = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::FRAGMENT, fragmentCode);

    VertexShader = Graphics::Vulkan::Shader::Create(device, vertexCompile);
    FragmentShader = Graphics::Vulkan::Shader::Create(device, fragmentCompile);
    RenderPass = Graphics::Vulkan::RenderPass::Create(device, swapchain.SurfaceFormat.format);
    Pipeline = Graphics::Vulkan::Pipeline::CreateGraphicsPipeline(device, VertexShader, FragmentShader, RenderPass, swapchain.Extent.width, swapchain.Extent.height);

    Framebuffers.resize(swapchain.Images.size());
    for (uint32_t i = 0; i < Framebuffers.size(); i++)
      Framebuffers[i] = Graphics::Vulkan::Framebuffer::Create(device, swapchain.Extent.width, swapchain.Extent.height, RenderPass, {swapchain.Views[i]});
  
    Renderer = Graphics::Vulkan::Command::Create(device, GraphicsCommandPool, Graphics::Vulkan::Command::PRIMARY);
  }
}
Rendering::~Rendering()
{
  auto vulkan = Core::Engine::GetVulkan();
  auto device = Core::Engine::GetMainDevice();

  Graphics::Vulkan::Device::WaitForDevice(device);

  //general
  {
    Graphics::Vulkan::CommandPool::Destroy(TransferCommandPool);
    Graphics::Vulkan::CommandPool::Destroy(ComputeCommandPool);
    Graphics::Vulkan::CommandPool::Destroy(GraphicsCommandPool);
  }

  //graphics pipeline
  {
    Graphics::Vulkan::Pipeline::Destroy(Pipeline);
    Graphics::Vulkan::RenderPass::Destroy(RenderPass);
    Graphics::Vulkan::Shader::Destroy(FragmentShader);
    Graphics::Vulkan::Shader::Destroy(VertexShader);
  }
}
} // namespace Systems
} // namespace Tortuga
