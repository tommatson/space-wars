#include "game_object.hpp"



GameObject::GameObject(id_t objId) : id{objId}{


}


GameObject GameObject::createGameObject(){
  static id_t currentId = 0;
  return GameObject{currentId++};
}


GameObject::id_t GameObject::getId(){
  return id;
}
