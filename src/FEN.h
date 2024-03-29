#ifndef CHESS_ENGINE_FEN_H
#define CHESS_ENGINE_FEN_H

#include "Castle.h"
#include "Enums/Colors.h"
#include <iostream>
#include <thread>
#include <vector>

class FEN {
  public:
    explicit FEN() = default;

    static Color parseTurn(const std::string &);

    template <Color side>
    static Castle<side> parseCastle(const std::string &);

    static uint64_t parseEnPassant(const std::string &);

    static uint8_t parseHalfmoveClock(const std::string &);

    static uint16_t parseFullmoveNumber(const std::string &);

    /**
     * Parse a FEN string
     *
     * @param[in] value fen string
     * @param[in] value target piece (Ex. "k", "K", "Q", "q" etc.)
     * @return bitboard
     */
    static uint64_t parsePiece(const std::string &, const char &);
};

#endif // CHESS_ENGINE_FEN_H
