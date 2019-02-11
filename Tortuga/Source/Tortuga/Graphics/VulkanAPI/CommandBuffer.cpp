#include "CommandBuffer.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
CommandBuffer::CommandBuffer(Device *device, CommandPool *commandPool, uint32_t amount, bool isPrimary)
{
    this->_isPrimary = isPrimary;
    this->_commandPool = commandPool;
    this->_device = device;
    this->_commandBuffers.resize(amount);

    auto allocInfo = VkCommandBufferAllocateInfo();
    {
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = _commandPool->GetCommandPool();

        if (isPrimary)
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        else
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;

        allocInfo.commandBufferCount = _commandBuffers.size();
    }
    if (vkAllocateCommandBuffers(_device->GetVirtualDevice(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS)
    {
        Console::Fatal("Failed to allocate command buffer on device!");
    }
}

CommandBuffer::~CommandBuffer()
{
    vkFreeCommandBuffers(_device->GetVirtualDevice(), _commandPool->GetCommandPool(), _commandBuffers.size(), _commandBuffers.data());
}

void CommandBuffer::BeginCommandBuffer(uint32_t index)
{
    auto beginInfo = VkCommandBufferBeginInfo();
    {
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr;
    }

    if (vkBeginCommandBuffer(_commandBuffers[index], &beginInfo) != VK_SUCCESS)
    {
        Console::Fatal("Failed to begin command buffer!");
    }
}

void CommandBuffer::BeginCommandBuffer(uint32_t index, RenderPass *renderPass, uint32_t subPass)
{
    auto inheritanceInfo = VkCommandBufferInheritanceInfo();
    {
        inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
        inheritanceInfo.renderPass = renderPass->GetRenderPass();
        inheritanceInfo.subpass = subPass;
    }

    auto beginInfo = VkCommandBufferBeginInfo();
    {
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
        beginInfo.pInheritanceInfo = &inheritanceInfo;
    }

    if (vkBeginCommandBuffer(_commandBuffers[index], &beginInfo) != VK_SUCCESS)
    {
        Console::Fatal("Failed to begin command buffer!");
    }
}

void CommandBuffer::EndCommandBuffer(uint32_t index)
{
    if (vkEndCommandBuffer(_commandBuffers[index]) != VK_SUCCESS)
    {
        Console::Fatal("Failed to end command buffer");
    }
}

void CommandBuffer::BeginRenderPass(uint32_t index, Swapchain *swapchain, RenderPass *renderPass, VkFramebuffer framebuffer, VkSubpassContents subPassFlags)
{
    auto renderPassInfo = VkRenderPassBeginInfo();
    {
        std::vector<VkClearValue> clearColor(2);
        {
            clearColor[0] = {0.0f, 0.0f, 0.0f, 1.0f};
            clearColor[1] = {1.0f, 0};
        }

        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass->GetRenderPass();
        renderPassInfo.framebuffer = framebuffer;
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapchain->GetExtent2D();
        renderPassInfo.clearValueCount = clearColor.size();
        renderPassInfo.pClearValues = clearColor.data();
    }
    vkCmdBeginRenderPass(_commandBuffers[index], &renderPassInfo, subPassFlags);
}

void CommandBuffer::EndRenderPass(uint32_t index)
{
    vkCmdEndRenderPass(_commandBuffers[index]);
}

void CommandBuffer::BindPipeline(uint32_t index, Pipeline *pipeline)
{
    vkCmdBindPipeline(_commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetVulkanPipeline());
}

void CommandBuffer::CreateDrawCommand(uint32_t index, Buffer *vertexBuffer, Buffer *indexBuffer, uint32_t indicesSize)
{
    std::vector<VkBuffer> buffers = {vertexBuffer->GetBuffer()};
    std::vector<VkDeviceSize> offsets = {0};

    vkCmdBindVertexBuffers(_commandBuffers[index], 0, 1, buffers.data(), offsets.data());
    vkCmdBindIndexBuffer(_commandBuffers[index], indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
    vkCmdDrawIndexed(_commandBuffers[index], indicesSize, 1, 0, 0, 0);
}

void CommandBuffer::BindDescriptorSet(uint32_t index, PipelineLayout *pipelineLayout, std::vector<VkDescriptorSet> descriptorSets)
{
    vkCmdBindDescriptorSets(_commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout->GetPipelineLayout(), 0, descriptorSets.size(), descriptorSets.data(), 0, nullptr);
}
void CommandBuffer::SetViewport(uint32_t index, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    auto viewport = VkViewport();
    {
        viewport.x = x;
        viewport.y = y;
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
    }
    vkCmdSetViewport(_commandBuffers[index], 0, 1, &viewport);
}
void CommandBuffer::SetScissor(uint32_t index, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    auto rect = VkRect2D();
    {
        rect.offset.x = x;
        rect.offset.y = y;
        rect.extent.width = width;
        rect.extent.height = height;
    }
    vkCmdSetScissor(_commandBuffers[index], 0, 1, &rect);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga