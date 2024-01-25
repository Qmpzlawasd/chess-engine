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
    Piece(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : white{whiteConfiguration}, black{blackConfiguration} {};
    [[nodiscard]] uint64_t getWhite() const { return white; }
    [[nodiscard]] uint64_t getBlack() const { return black; }
};

class Pawn : public Piece {
  public:
    Pawn(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : Piece{whiteConfiguration, blackConfiguration} {};
};

class Queen : public Piece {
  public:
    Queen(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : Piece{whiteConfiguration, blackConfiguration} {};
};

class Rook : public Piece {
  public:
    Rook(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : Piece{whiteConfiguration, blackConfiguration} {};
    static uint64_t getNaiveAttackPattern(const Square &square);
};

class Bishop : public Piece {
  public:
    Bishop(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : Piece{whiteConfiguration, blackConfiguration} {};
    static uint64_t getNaiveAttackPattern(const Square &square);
};

class King : public Piece {
  public:
    King(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : Piece{whiteConfiguration, blackConfiguration} {};
};

class Knight : public Piece {
  public:
    Knight(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : Piece{whiteConfiguration, blackConfiguration} {};
};

#endif // CHESS_ENGINE_PIECE_H
