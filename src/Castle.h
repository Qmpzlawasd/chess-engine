#ifndef CHESS_ENGINE_CASTLE_H
#define CHESS_ENGINE_CASTLE_H

#include "Enums/Colors.h"
#include "Enums/Sides.h"
#include "Enums/Squares.h"

#include <cstdint>

template <Color side>
class Castle {
    bool queenSideRook;
    bool kingSideRook;

  public:
    explicit Castle(const bool &_kingRookMoved, const bool &_queenRookMoved) noexcept
        : queenSideRook{_queenRookMoved}, kingSideRook{_kingRookMoved} {};

    template <Side area>
    [[nodiscard]] bool hasRookMoved() const noexcept {
        if constexpr (area == QUEEN_SIDE) {
            return queenSideRook;
        }
        return kingSideRook;
    }

    template <Side area>
    void rookMoved() noexcept {
        if constexpr (area == QUEEN_SIDE) {
            queenSideRook = false;
        } else {
            kingSideRook = false;
        }
    }

    void kingMoved() noexcept {
        queenSideRook = false;
        kingSideRook = false;
    }
};

#endif // CHESS_ENGINE_CASTLE_H