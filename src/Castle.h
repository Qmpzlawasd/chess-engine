#pragma once

#include "Colors.h"
#include "Squares.h"
#include "Utils.h"

template <Color side>
class Castle {
    uint64_t queenSideRook;
    uint64_t kingSideRook;

  public:
    explicit Castle(const bool &_kingRookMoved, const bool &_queenRookMoved) noexcept
        : queenSideRook{_queenRookMoved ? 0 : (side == WHITE ? Utils::setSquare(A1) : Utils::setSquare(A8))},
          kingSideRook{_kingRookMoved ? 0 : (side == WHITE ? Utils::setSquare(H1) : Utils::setSquare(H8))} {};
};