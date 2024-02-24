#ifndef CHESS_ENGINE_PIECE_H
#define CHESS_ENGINE_PIECE_H

#include "Colors.h"
#include "Squares.h"
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

    [[nodiscard]] uint64_t getWhite() const { return whitePieces; }

    [[nodiscard]] uint64_t getBlack() const { return blackPieces; }
};

class SpecialPiece : public Piece {
  public:
    SpecialPiece(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece(whiteConfiguration, blackConfiguration){};
};

class Pawn : public SpecialPiece {
  public:
    Pawn(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : SpecialPiece(whiteConfiguration, blackConfiguration){};
    Pawn() : SpecialPiece(0, 0){};

    template <Color side>
    [[nodiscard]]  static uint64_t getMoves(const Square &) noexcept;
};

#endif // CHESS_ENGINE_PIECE_H
