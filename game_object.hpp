#pragma once


#include "model.hpp"


#include <memory>


struct Transform2dComponent {
  glm::vec2 translation{};
  glm::mat2 mat2() {return glm::mat2{1.0f}; };
};

class GameObject{
public:
  using id_t = unsigned int;


  static GameObject createGameObject();

  GameObject(const GameObject&) = delete;
  GameObject &operator=(const GameObject&) = delete;

  // Move
  GameObject(GameObject&&) = default;
  GameObject &operator=(GameObject&&) = default;




  id_t getId();

  std::shared_ptr<Model> model{};
  glm::vec3 color{};

  Transform2dComponent transform2d{};

private:

  GameObject(id_t objId);
  id_t id;



};
