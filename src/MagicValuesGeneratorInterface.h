#pragma once

#include "MagicValues.h"
#include "Piece.h"
#include <bitset>
#include <thread>
#include <vector>

class MagicValuesGeneratorInterface {
  public:
    uint64_t naiveAttackPattern;
    uint8_t shiftValue;

    explicit MagicValuesGeneratorInterface(const uint8_t &_shiftValue, const uint64_t &_naiveAttackPattern = 0)
        : naiveAttackPattern{_naiveAttackPattern}, shiftValue{_shiftValue} {}

    virtual void generate() const noexcept = 0;

    [[nodiscard]] virtual std::vector<std::vector<uint64_t>> getTables(std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE>,
                                                                       const SlidingPiece &) const noexcept = 0;

    virtual ~MagicValuesGeneratorInterface() = default;

    void setNaiveAttackPattern(const uint64_t &_naiveAttackPattern) {
        MagicValuesGeneratorInterface::naiveAttackPattern = _naiveAttackPattern;
    }

    void setShiftValue(const uint8_t &_shiftValue) { MagicValuesGeneratorInterface::shiftValue = _shiftValue; }
};
