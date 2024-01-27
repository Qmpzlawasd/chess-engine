//
// Created by stefan on 25/01/24.
//

#ifndef CHESS_ENGINE_PIECE_H
#define CHESS_ENGINE_PIECE_H

#include "Squares.h"
#include "Utils.h"

class Piece {

    uint64_t white;
    uint64_t black;

  public:
    Piece() : white{0}, black{0} {};

    Piece(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : white{whiteConfiguration}, black{blackConfiguration} {};

    [[nodiscard]] uint64_t getWhite() const { return white; }

    [[nodiscard]] uint64_t getBlack() const { return black; }
};

class Pawn : public Piece {
  public:
    Pawn(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
};

class King : public Piece {
  public:
    King(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
};

class Knight : public Piece {
  public:
    Knight(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
};

class SlidingPiece : public Piece {
  public:
    SlidingPiece() : Piece(){};

    SlidingPiece(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece(whiteConfiguration, blackConfiguration){};

    [[nodiscard]] virtual uint64_t fillPositions(const Square &, const uint64_t &) const noexcept = 0;
};

class Rook : public SlidingPiece {
  public:
    Rook() : SlidingPiece{} {};

    Rook(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : SlidingPiece{whiteConfiguration, blackConfiguration} {};

    static uint64_t getNaiveAttackPattern(const Square &square);

    [[nodiscard]] uint64_t fillPositions(const Square &square, const uint64_t &pattern) const noexcept override;
};

class Bishop : public SlidingPiece {
  public:
    Bishop() : SlidingPiece{} {};

    Bishop(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : SlidingPiece{whiteConfiguration, blackConfiguration} {};

    static uint64_t getNaiveAttackPattern(const Square &square);

    [[nodiscard]] uint64_t fillPositions(const Square &square, const uint64_t &pattern) const noexcept override;
};

class Queen : public SlidingPiece {
  public:
    Queen() : SlidingPiece{} {};

    Queen(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : SlidingPiece{whiteConfiguration, blackConfiguration} {};

    [[nodiscard]] uint64_t fillPositions(const Square &square, const uint64_t &pattern) const noexcept override;
};

#endif // CHESS_ENGINE_PIECE_H
