#ifndef CHESS_ENGINE_UTILS_H
#define CHESS_ENGINE_UTILS_H

#include "Enums/Colors.h"
#include "Enums/Squares.h"

#include <functional>
#include <iostream>
#include <thread>
#include <x86gprintrin.h>

class Utils {
    static void showReversedBin(int8_t);

  public:
    static constexpr uint8_t ROW_NUMBER = 8;
    static constexpr uint8_t COLUMN_NUMBER = 8;

    static constexpr uint8_t NUMBER_SQUARES_TABLE = ROW_NUMBER * COLUMN_NUMBER;
    static constexpr uint8_t NUMBER_PIECE_TYPES = 6;

    static constexpr uint64_t A_FILE = 0x0101010101010101;
    static constexpr uint64_t H_FILE = 0x8080808080808080;

    static constexpr uint64_t FIRST_ROW = 0x00000000000000ff;
    static constexpr uint64_t LAST_ROW = 0xff00000000000000;

    static constexpr uint64_t PAWNS_STARTER_WHITE = FIRST_ROW << ROW_NUMBER;
    static constexpr uint64_t PAWNS_STARTER_BLACK = LAST_ROW >> ROW_NUMBER;

    static constexpr uint64_t DOUBLE_PUSH_RANK_WHITE = FIRST_ROW << ROW_NUMBER * 3;
    static constexpr uint64_t DOUBLE_PUSH_RANK_BLACK = LAST_ROW >> ROW_NUMBER * 3;

    static constexpr uint64_t ENPASSANT_RANK_WHITE = FIRST_ROW << ROW_NUMBER * 2;
    static constexpr uint64_t ENPASSANT_RANK_BLACK = LAST_ROW >> ROW_NUMBER * 2;

    static constexpr uint64_t CHECK_ENPASSANT_WHITE = FIRST_ROW << ROW_NUMBER * 4;
    static constexpr uint64_t CHECK_ENPASSANT_BLACK = LAST_ROW >> ROW_NUMBER * 4;

    static constexpr Square QUEEN_SIDE_CASTLE_WHITE = C1;
    static constexpr Square KING_SIDE_CASTLE_WHITE = G1;
    static constexpr Square QUEEN_SIDE_CASTLE_BLACK = C8;
    static constexpr Square KING_SIDE_CASTLE_BLACK = G8;

    [[nodiscard]] static Square popLSB(uint64_t &board) {
        const auto poppedBit = static_cast<const Square>(_tzcnt_u64(board));
        board = _blsr_u64(board);
        return poppedBit;
    }

    [[nodiscard]] static Square popLSBCopy(uint64_t board) { return static_cast<const Square>(_tzcnt_u64(board)); }

    static void runForEachSetBit(uint64_t bitboard, const std::function<void(const Square &)> &callEachBit) {
        for (; bitboard; bitboard = _blsr_u64(bitboard)) {
            const auto poppedBit = static_cast<const Square>(_tzcnt_u64(bitboard));
            callEachBit(poppedBit);
        }
    }

    [[nodiscard]] static consteval Color flipColor(const Color &color) {
        if (color == WHITE) {
            return BLACK;
        }
        return WHITE;
    };

    static void showBitBoard(const uint64_t &);

    static void showBitBoard(const Square &);

    /**
     * @param[in] value bitboard
     * @param[in] value in rage from 0 to 7
     */
    static uint64_t getIthFile(const uint64_t &, const int &);

    /**
     * @param[in] value bitboard
     * @param[in] value in rage from 0 to 7
     */
    static uint64_t setSquare(const Square &);

    static uint64_t setSquare(const std::vector<Square> &squares);

    /**
     * @param[in] value bitboard
     * @param[in] value in rage from 0 to 7
     */
    static uint64_t getIthRank(const uint64_t &, const int &);

    static uint64_t getSetLineBetween(const Square &, const Square &);

    static std::string_view squareToString(const Square &) noexcept;
};

#endif // CHESS_ENGINE_UTILS_H
