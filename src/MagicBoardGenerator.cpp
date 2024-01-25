#pragma once
#include "MagicValues.cpp"
#include "Squares.cpp"
#include "Utils.cpp"
#include <thread>
#include <unordered_map>
#include <vector>

class MagicBoardGenerator {
  public:
    virtual const std::vector<uint64_t> generate() const noexcept = 0;
    virtual const std::vector<std::vector<uint64_t>>
    getTables(const uint64_t &,
              const uint64_t[] = ROOK_CONSTANTS,
              const uint8_t & = ROOK_SHIFT) const noexcept = 0;
    virtual ~MagicBoardGenerator() = default;
};
