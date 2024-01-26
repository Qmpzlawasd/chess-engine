#pragma once

#include "MagicValues.h"
#include <bitset>
#include <thread>
#include <vector>

class MagicValuesGeneratorInterface {
  public:
    uint64_t naiveAttackPattern;
    uint8_t shiftValue;

    virtual void generate() const noexcept = 0;

    [[nodiscard]] virtual std::vector<std::vector<uint64_t>>
        getTables(std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE>) const noexcept = 0;

    virtual ~MagicValuesGeneratorInterface() = default;
};
