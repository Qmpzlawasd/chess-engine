#ifndef CHESS_ENGINE_JUMPINGPIECE_H
#define CHESS_ENGINE_JUMPINGPIECE_H

#include "../Squares.h"
#include "Piece.h"
#include <cstdint>
class JumpingPiece : public Piece {
  public:
    JumpingPiece(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece(whiteConfiguration, blackConfiguration){};
};

class King : public JumpingPiece {

  public:
    King(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : JumpingPiece(whiteConfiguration, blackConfiguration){};
    King() : JumpingPiece(0, 0){};

    [[nodiscard]] static uint64_t getMoves(const Square &) noexcept;
    [[nodiscard]] static uint64_t getMoves(const uint64_t &) noexcept;
};

class Knight : public JumpingPiece {
  public:
    Knight(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : JumpingPiece(whiteConfiguration, blackConfiguration){};
    Knight() : JumpingPiece(0, 0){};

    [[nodiscard]] static uint64_t getMoves(const Square &square) noexcept;
};
#endif // CHESS_ENGINE_JUMPINGPIECE_H
