#ifndef CHESS_ENGINE_PIECE_H
#define CHESS_ENGINE_PIECE_H

#include "Colors.h"
#include "Enums/Squares.h"
#include "Utils.h"
#include <array>

class Piece {
  protected:
    uint64_t whitePieces;
    uint64_t blackPieces;

  public:
    Piece() : whitePieces{0}, blackPieces{0} {};

    Piece(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : whitePieces{whiteConfiguration}, blackPieces{blackConfiguration} {};

    template <Color side>
    [[nodiscard]] uint64_t getBitboard() const noexcept {
        if constexpr (side == WHITE) {
            return whitePieces;
        }
        return blackPieces;
    }

    virtual ~Piece() = default;
};

#endif // CHESS_ENGINE_PIECE_H
