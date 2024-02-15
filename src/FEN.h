#ifndef CHESS_ENGINE_FEN_H
#define CHESS_ENGINE_FEN_H

#include "Squares.h"
#include <bitset>
#include <iostream>
#include <thread>
#include <vector>

class FEN {
  public:
    explicit FEN() = default;

    static bool parseTurn(const std::string &fen);

    static std::bitset<4> parseCastle(const std::string &fen);

    static uint64_t parseEnPassant(const std::string &fen);

    static uint8_t parseHalfmoveClock(const std::string &fen);

    static uint16_t parseFullmoveNumber(const std::string &fen);

    /**
     * Parse a FEN string
     *
     * @param[in] value fen string
     * @param[in] value target piece (Ex. "k", "K", "Q", "q" etc.)
     * @return bitboard
     */
    static uint64_t parsePiece(const std::string &fen, const char &piece);
};

#endif // CHESS_ENGINE_FEN_H
