#include "view_manager.hpp"


namespace Engine::UI::View{
  
ViewManager::ViewManager(std::unique_ptr<View> baseView){
  viewStack.push(std::move(baseView));
}

ViewManager::~ViewManager(){}

void ViewManager::pushView(std::unique_ptr<View> newView){
  viewStack.push(std::move(newView));
}

void ViewManager::popView(){
  if (!viewStack.empty()) {
    viewStack.pop();
  }
}

void ViewManager::render(){
  viewStack.top()->render();
}


} // namespace Engine::UI::Canvas 
