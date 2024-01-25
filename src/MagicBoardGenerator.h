#pragma once

#include "MagicValues.h"
#include <bitset>
#include <thread>
#include <vector>

class MagicBoardGenerator {
  public:
    [[nodiscard]] virtual std::vector<uint64_t> generate() const noexcept = 0;

    [[nodiscard]] virtual std::vector<std::vector<uint64_t>>
    getTables(const uint64_t &, const uint64_t[], const uint8_t &) const noexcept = 0;

    virtual ~MagicBoardGenerator() = default;
};
