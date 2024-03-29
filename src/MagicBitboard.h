#ifndef CHESS_ENGINE_MAGICBITBOARD_H
#define CHESS_ENGINE_MAGICBITBOARD_H


#include "MagicBitboard.h"
#include "MagicValuesGeneratorInterface.h"
#include "MagicValuesParallelGenerator.h"
#include "Piece/SlidingPiece.h"
#include <memory>

class MagicBitboard {
  public:
    std::shared_ptr<MagicValuesGeneratorInterface> magicGenerator;
    const std::vector<std::vector<uint64_t>> rookMoveTable;
    const std::vector<std::vector<uint64_t>> bishopMoveTable;

    explicit MagicBitboard(const std::shared_ptr<MagicValuesGeneratorInterface> &_magicGenerator)
        : magicGenerator{_magicGenerator}, 
          rookMoveTable{_magicGenerator->getTables(Rook::MAGIC_CONSTANTS, Rook{})},
          bishopMoveTable{_magicGenerator->getTables(Bishop::MAGIC_CONSTANTS, Bishop{})} {};
};

static const MagicBitboard magicBitboard{std::make_shared<MagicValuesParallelGenerator>(MagicValuesParallelGenerator{})};

#endif // CHESS_ENGINE_MAGICBITBOARD_H