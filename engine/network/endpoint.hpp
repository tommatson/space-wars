#pragma once

#include <cstdint>
#include <array>

namespace Engine::Network {

struct Endpoint {
  std::array<uint8_t , 16> ip; 
  uint16_t port;
};

}
