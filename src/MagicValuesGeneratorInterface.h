#pragma once

#include "Piece/Piece.h"
#include "Piece/SlidingPiece.h"
#include <bitset>
#include <thread>
#include <vector>

class MagicValuesGeneratorInterface {
  public:
    virtual void generate(const SlidingPiece &) const noexcept = 0;

    [[nodiscard]] virtual std::vector<std::vector<uint64_t>> getTables(const std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE> &,
                                                                       const SlidingPiece &) const noexcept = 0;

    virtual ~MagicValuesGeneratorInterface() = default;
};
