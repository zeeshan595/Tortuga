#include "./Shader.hpp"

#include <regex>
#include <vector>
#include "../../Utils/IO.hpp"
#include "../../Core/GUID.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Shader
{
std::string GetFileLocation(std::string file)
{
  size_t found;
  found = file.find_last_of("/");
  return file.substr(0, found);
}
std::string GetFullShaderCode(std::string location, std::string code)
{
  std::string fullShader = code;
  std::regex pattern("#include[ ]*\"([a-zA-Z0-9./]+)\"");
  std::smatch matches;
  while (std::regex_search(fullShader, matches, pattern))
  {
    if (matches.size() < 2)
      continue;
    const auto fullMatch = matches[0].str();
    const auto includeFile = matches[1].str();
    const auto includedShader = Utils::IO::GetFileContents(location + includeFile).data();
    const auto includeCode = GetFullShaderCode(GetFileLocation(location + includeFile), includedShader);
    const auto includeIndex = fullShader.find(fullMatch);
    fullShader.replace(includeIndex, fullMatch.length(), includeCode);
  }
  return fullShader;
}
std::vector<char> CompileShader(
    Instance::Instance instance,
    ShaderType type,
    std::vector<char> code,
    std::string location)
{
  const auto fullShaderCode = GetFullShaderCode(location, code.data());
  std::string typeString = "";
  switch (type)
  {
  case ShaderType::COMPUTE:
    typeString = "comp";
    break;
  }
  auto fullShaderChar = std::vector<char>(fullShaderCode.length());
  memcpy(fullShaderChar.data(), fullShaderCode.c_str(), fullShaderCode.length());
  const auto tempShaderPath = location + Core::GUID::GenerateGUID(1) + "." + typeString;
  Utils::IO::SetFileContents(tempShaderPath, fullShaderChar);

  const std::string cmd = "usr/bin/glslangValidator -V " + tempShaderPath + " -o " + tempShaderPath + ".spv";
  system(cmd.c_str());
  const auto compiledShader = Utils::IO::GetFileContents(tempShaderPath + ".spv");
  const std::string cleanUpCommand = "rm " + tempShaderPath + " && rm " + tempShaderPath + ".spv";
  system(cleanUpCommand.c_str());

  //todo compile shader
  return compiledShader;
}
Shader Create(Device::Device device, std::vector<char> compiled)
{
  Shader data = {};
  data.Device = device.Device;

  VkShaderModuleCreateInfo createInfo = {};
  {
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = compiled.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(compiled.data());
  }
  ErrorCheck::Callback(vkCreateShaderModule(device.Device, &createInfo, nullptr, &data.Shader));

  return data;
}

void Destroy(Shader data)
{
  if (data.Shader == VK_NULL_HANDLE)
    return;

  vkDestroyShaderModule(data.Device, data.Shader, nullptr);
}
} // namespace Shader
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga