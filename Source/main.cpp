#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  Core::Screen::SetWindowTitle("Hello World");
  Core::Screen::ResizeWindow(1920, 1080);

  //Start rendering system
  Core::CreateSystem<Systems::Rendering>();

  //create cube entity
  auto cube = Core::Entity::Create();
  {
    //transform data
    auto transform = cube->AddComponent<Component::Transform>();
    transform->Position = glm::vec3(0, 0, 10);
    transform->Rotation = glm::vec4(0, 0, 0, 1);
    transform->Scale = glm::vec3(1, 1, 1);

    //mesh data
    auto model = Utils::IO::LoadObjFile("Models/Cube.obj");
    auto mesh = cube->AddComponent<Component::Mesh>();
    mesh->SetVertices(model.Vertices, true);
    mesh->SetNormals(model.Normals);
    mesh->SetTextures(model.Textures);
    mesh->SetIndices(model.Indices);
    mesh->SetDynamic();
  }

  //create light entity
  auto light = Core::Entity::Create();
  {
    auto transform = light->AddComponent<Component::Transform>();
    transform->Position = glm::vec4(1, 5, 10, 1);
    transform->Rotation = glm::vec4(0, 0, 0, 1);
    transform->Scale = glm::vec4(1, 1, 1, 1);

    auto lightComponent = light->AddComponent<Component::Light>();
    lightComponent->Intensity = 1.0f;
    lightComponent->Range = 10.0f;
  }

  float yPosition = 0.0f;

  //Main Loop
  bool shouldClose = false;
  while (!shouldClose)
  {
    //Window events
    auto event = Core::Screen::PollEvents();
    //Quit on close pressed
    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
      shouldClose = true;

    cube->GetComponent<Component::Transform>()->Rotation = glm::vec4(0, yPosition, 0, 1);

    //iterate through all system and execute update functions
    Core::IterateSystemLoop();
    yPosition += 0.00001;
  }

  //destroy cube
  Core::Entity::Destroy(cube);

  //destroy rendering system
  Core::DestroySystem<Systems::Rendering>();
  return 0;
}