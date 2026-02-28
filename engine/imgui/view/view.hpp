#pragma once

#include "imgui.h"


namespace Engine::UI::View{
  
class View {
public:
  View() = default;
  View(const View&) = delete;
  View& operator=(const View&) = delete;

  virtual void render() = 0;
  virtual ~View() = default; 


};




} // namespace Engine:
