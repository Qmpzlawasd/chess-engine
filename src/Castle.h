#pragma once

#include "Colors.h"
#include "Squares.h"
#include "Utils.h"

template <Color side>
class Castle {
    uint64_t queenSide;
    uint64_t kingSide;

    uint64_t queenRookMoved;
    uint64_t kingRookMoved;

  public:
    Castle(const Castle &&castle) noexcept
        : queenSide{castle.queenSide}, kingSide{castle.kingSide}, queenRookMoved{castle.queenRookMoved},
          kingRookMoved{castle.kingRookMoved} {};

    Castle() noexcept
        : queenSide{(side == WHITE ? Utils::setSquare(C1) : Utils::setSquare(C8))},
          kingSide{(side == WHITE ? Utils::setSquare(G1) : Utils::setSquare(G8))},
          queenRookMoved{(side == WHITE ? Utils::setSquare(A1) : Utils::setSquare(A8))},
          kingRookMoved{(side == WHITE ? Utils::setSquare(H1) : Utils::setSquare(H8))} {};

    explicit Castle(const bool &_kingRookMoved, const bool &_queenRookMoved) noexcept
        : queenSide{(side == WHITE ? Utils::setSquare(C1) : Utils::setSquare(C8))},
          kingSide{(side == WHITE ? Utils::setSquare(G1) : Utils::setSquare(G8))},
          queenRookMoved{(_queenRookMoved ? 0 : ((side == WHITE) ? Utils::setSquare(A1) : Utils::setSquare(A8)))},
          kingRookMoved{(_kingRookMoved ? 0 : ((side == WHITE) ? Utils::setSquare(H1) : Utils::setSquare(H8)))} {};
};