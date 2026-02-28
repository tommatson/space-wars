#pragma once

#include "view.hpp"

#include <stack>
#include <memory>


namespace Engine::UI::View{
  
class ViewManager {
public:

  ViewManager(std::unique_ptr<View> baseView);
  ~ViewManager();

  ViewManager(const ViewManager& ) = delete;
  ViewManager &operator=(const ViewManager&) = delete;

  void pushView(std::unique_ptr<View> newView);

  void popView();

  void render();

protected:

  std::stack<std::unique_ptr<View>> viewStack;

};




} // namespace Engine::UI::Canvas 
