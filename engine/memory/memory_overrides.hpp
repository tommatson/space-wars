#pragma once

#include <cstddef>
#include <cassert>

// -----------------------------------------------------------------------------
// Core API
// -----------------------------------------------------------------------------
void* operator new (std::size_t size);
void* operator new[](std::size_t size);

void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

