#ifndef _MESH
#define _MESH

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "../Graphics/Vulkan/DescriptorLayout.hpp"
#include "../Graphics/Vulkan/DescriptorPool.hpp"
#include "../Graphics/Vulkan/DescriptorSets.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/CommandPool.hpp"
#include "../Graphics/Vulkan/Command.hpp"

#include "../Core/Engine.hpp"
#include "../Console.hpp"
#include "../Graphics/Vertex.hpp"

namespace Tortuga
{
namespace Component
{
#define MAX_VERTICES_SIZE 65535
#define MAX_INDICES_SIZE 65535

struct MeshBufferData
{
  uint32_t VerticesSize;
  uint32_t IndicesSize;
  uint32_t Reserved1;
  float Bounds;
  glm::vec4 Center;
  glm::mat4 Transformation;
  glm::mat4 NormalMatrix;
  Graphics::Vertex Vertices[MAX_VERTICES_SIZE];
  uint32_t Indices[MAX_INDICES_SIZE];
};
const uint32_t MESH_SIZE = sizeof(MeshBufferData);
struct Mesh
{
  bool IsStatic;
  bool IsProcessedOnce;
  MeshBufferData BufferData;
  Graphics::Vulkan::DescriptorPool::DescriptorPool DescriptorPool;
  Graphics::Vulkan::DescriptorSets::DescriptorSets DescriptorSets;
  Graphics::Vulkan::Buffer::Buffer Staging;
  Graphics::Vulkan::Buffer::Buffer Buffer;
  Graphics::Vulkan::CommandPool::CommandPool CommandPool;
  Graphics::Vulkan::Command::Command Command;

  void ResetTransformation();
  void ApplyTransformation(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale);
  void SetVertices(std::vector<Graphics::Vertex> vertices, bool recalculateBounds = false);
  void SetIndices(std::vector<uint32_t> indices);
  void SetStatic();
  void SetDynamic();

  Mesh();
  ~Mesh();
};
Graphics::Vulkan::DescriptorLayout::DescriptorLayout GetMeshDescriptorLayout();
} // namespace Component
} // namespace Tortuga

#endif