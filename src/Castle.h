#pragma once

#include "Enums/Sides.h"
#include "Enums/Colors.h"
#include "Enums/Squares.h"

#include <cstdint>

template <Color side>
class Castle {
    bool queenSideRook;
    bool kingSideRook;

  public:
    explicit Castle(const bool &_kingRookMoved, const bool &_queenRookMoved) noexcept
        : queenSideRook{_queenRookMoved},
          kingSideRook{_kingRookMoved} {
    };

    template <Side area>
    [[nodiscard]] bool hasRookMoved() const noexcept {
        if constexpr (area == QUEEN_SIDE) {
            return queenSideRook;
        }
        return kingSideRook;
    }

    void kingMoved() noexcept {
        queenSideRook = false;
        kingSideRook = false;
    }

    template <Side area>
    [[nodiscard]] uint64_t getCastleRights() const noexcept {
        if constexpr (area == QUEEN_SIDE) {
            return queenSideRook;
        }
        return kingSideRook;
    }
};