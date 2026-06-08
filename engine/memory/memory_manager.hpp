#pragma once

#include "linear_allocator.hpp"
#include "pool_allocator.hpp"
#include "system_arena.hpp"

#include <memory>

namespace Engine::Memory {

class MemoryManager {
public:

  // ---------------------------------------------------------------------------
  // Leaders
  // ---------------------------------------------------------------------------
  MemoryManager() = default;

  // ---------------------------------------------------------------------------
  // Deleted Semantics
  // ---------------------------------------------------------------------------
  ~MemoryManager() = delete;
  MemoryManager(const MemoryManager&) = delete;
  MemoryManager& operator=(const MemoryManager&) = delete;
  MemoryManager(MemoryManager&&) = delete;
  MemoryManager& operator=(MemoryManager&&) = delete;

  // ---------------------------------------------------------------------------
  // Core API
  // ---------------------------------------------------------------------------
  void init(std::size_t totalSize = 256 * 1024 * 1024) // Default 256 MB
  {
    mainArena_ = std::make_unique<SystemArena>(totalSize);

    // Allocator proportions of the arena allocator
    constexpr std::size_t PERSISTENT_PCT = 15;
    constexpr std::size_t FRAME_PCT = 25;
    constexpr std::size_t SCRATCH_PCT = 10;

    std::size_t persistentSize = (totalSize * PERSISTENT_PCT) / 100;
    std::size_t frameSize  = (totalSize * FRAME_PCT) / 100;
    std::size_t scratchSize = (totalSize * SCRATCH_PCT) / 100; 
    std::size_t poolSize  = totalSize - (persistentSize + frameSize + scratchSize);

    persistent_allocator_ = std::make_unique<LinearAllocator>(mainArena_->push_memory(persistentSize), persistentSize);
    frame_allocator_ = std::make_unique<LinearAllocator>(mainArena_->push_memory(frameSize), frameSize);
    scratch_allocator_ = std::make_unique<LinearAllocator>(mainArena_->push_memory(scratchSize), scratchSize);
    pool_allocator_ = std::make_unique<PoolAllocator>(mainArena_->push_memory(poolSize), poolSize, 64);
  }

  [[nodiscard]] LinearAllocator& getPersistantAllocator() const noexcept { return *persistent_allocator_; }
  void clearPersistantAllocator() { persistent_allocator_->clear(); }

  [[nodiscard]] LinearAllocator& getFrameAllocator() const noexcept { return *frame_allocator_; }
  void clearFrameAllocator() { frame_allocator_->clear(); }

  [[nodiscard]] LinearAllocator& getScratchAllocator() const noexcept { return *scratch_allocator_; }
  void clearScratchAllocator() { scratch_allocator_->clear(); }

  [[nodiscard]] PoolAllocator& getPoolAllocator() const noexcept { return *pool_allocator_; }
private:
  std::unique_ptr<SystemArena> mainArena_;
  std::unique_ptr<LinearAllocator> persistent_allocator_;
  std::unique_ptr<LinearAllocator> frame_allocator_;
  std::unique_ptr<LinearAllocator> scratch_allocator_;
  std::unique_ptr<PoolAllocator> pool_allocator_;
};
} // namespace Engine::Memory
