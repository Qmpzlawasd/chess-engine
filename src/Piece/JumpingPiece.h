#ifndef CHESS_ENGINE_JUMPINGPIECE_H
#define CHESS_ENGINE_JUMPINGPIECE_H

#include "../Squares.h"
#include "Piece.h"
#include <cstdint>
class JumpingPiece : public Piece {
  public:
    JumpingPiece(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece(whiteConfiguration, blackConfiguration){};
    [[nodiscard]] static uint64_t getMoves(const Square &) noexcept {
        puts("PANIC, NOT IMPLEMENTED STATIC GETNAIVEATTACKPATTERN");
        return -1;
    };
};

class King : public JumpingPiece {
  public:
    [[nodiscard]] static uint64_t getMoves(const Square &square) noexcept;
    King(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : JumpingPiece(whiteConfiguration, blackConfiguration){};
    King() : JumpingPiece(0, 0){};
};

class Knight : public JumpingPiece {
  public:
    Knight(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : JumpingPiece(whiteConfiguration, blackConfiguration){};
    Knight() : JumpingPiece(0, 0){};

    [[nodiscard]] static uint64_t getMoves(const Square &square) noexcept;
};
#endif // CHESS_ENGINE_JUMPINGPIECE_H
