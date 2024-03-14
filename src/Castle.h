#pragma once

#include "Colors.h"
#include "Enums/Sides.h"
#include "Enums/Squares.h"
#include "Utils.h"

template <Color side>
class Castle {
    uint64_t queenSideRook;
    uint64_t kingSideRook;

  public:
    explicit Castle(const bool &_kingRookMoved, const bool &_queenRookMoved) noexcept
        : queenSideRook{_queenRookMoved ? 0 : (side == WHITE ? Utils::setSquare(A1) : Utils::setSquare(A8))},
          kingSideRook{_kingRookMoved ? 0 : (side == WHITE ? Utils::setSquare(H1) : Utils::setSquare(H8))} {};

    template <Side area>
    [[nodiscard]] bool hasRookMoved() const noexcept {
        if constexpr (area == QUEEN_SIDE) {
            return queenSideRook != 0;
        }
        return kingSideRook != 0;
    }
    template <Side area>
    [[nodiscard]] uint64_t getCastleRights() const noexcept {
        if constexpr (area == QUEEN_SIDE) {
            return queenSideRook != 0;
        }
        return kingSideRook != 0;
    }
};