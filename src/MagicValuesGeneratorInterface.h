#ifndef CHESS_ENGINE_MAGICVALUESGENERATORINTERFACE_H
#define CHESS_ENGINE_MAGICVALUESGENERATORINTERFACE_H

#include "Piece/SlidingPiece.h"

class MagicValuesGeneratorInterface {
  public:
    virtual void generate(const SlidingPiece &) const noexcept = 0;

    [[nodiscard]] virtual std::vector<std::vector<uint64_t>> getTables(const std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE> &,
                                                                       const SlidingPiece &) const noexcept = 0;

    virtual ~MagicValuesGeneratorInterface() = default;
};

#endif // CHESS_ENGINE_MAGICVALUESGENERATORINTERFACE_H