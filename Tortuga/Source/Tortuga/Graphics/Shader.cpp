#include "Shader.h"

namespace Tortuga
{
namespace Graphics
{
Shader CreateShaderFromFile(HardwareController hardware, std::string filePath, ShaderType type)
{
  std::ifstream file(filePath, std::ios::ate | std::ios::binary);
  if (!file.is_open())
  {
    Console::Error("Failed to open file: {0}", Console::Arguments() << filePath);
    return {};
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  return CreateShader(hardware, buffer, type);
}

Shader CreateShader(HardwareController hardware, std::vector<char> code, ShaderType type)
{
  auto data = Shader();

  VkShaderStageFlagBits shaderFlags;
  switch (type)
  {
  case ShaderType::SHADER_TYPE_VERTEX:
    shaderFlags = VK_SHADER_STAGE_VERTEX_BIT;
    break;
  case ShaderType::SHADER_TYPE_FRAGMENT:
    shaderFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    break;
  case ShaderType::SHADER_TYPE_GEOMETRY:
    shaderFlags = VK_SHADER_STAGE_GEOMETRY_BIT;
    break;
  case ShaderType::SHADER_TYPE_COMPUTE:
    shaderFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    break;
  default:
    shaderFlags = VK_SHADER_STAGE_ALL;
    break;
  }

  data.VulkanShader.resize(hardware.Devices.size());
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
  {
    data.VulkanShader[i] = VulkanAPI::CreateShader(hardware.Devices[i].VulkanDevice, code, shaderFlags);
  }

  return data;
}
void DestroyShader(Shader data)
{
  for (uint32_t i = 0; i < data.VulkanShader.size(); i++)
  {
    VulkanAPI::DestroyShader(data.VulkanShader[i]);
  }
}
}; // namespace Graphics
}; // namespace Tortuga