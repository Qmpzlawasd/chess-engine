#ifndef CHESS_ENGINE_SPECIALPIECE_H
#define CHESS_ENGINE_SPECIALPIECE_H

#include "../Squares.h"
#include "Piece.h"
#include <cstdint>

class SpecialPiece : public Piece {
  public:
    SpecialPiece(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece(whiteConfiguration, blackConfiguration){};
    SpecialPiece() : Piece(){};
};

class Pawn : public SpecialPiece {
  public:
    Pawn(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : SpecialPiece(whiteConfiguration, blackConfiguration){};
    Pawn() : SpecialPiece(){};

    template <Color side>
    [[nodiscard]] static uint64_t getMoves(const Square &, const uint64_t &) noexcept;
    template <Color side>
    [[nodiscard]] static uint64_t getThreatens(const Square &square) noexcept;
};

#endif // CHESS_ENGINE_SPECIALPIECE_H
