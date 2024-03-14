#ifndef CHESS_ENGINE_UTILS_H
#define CHESS_ENGINE_UTILS_H

#include "Colors.h"
#include "Squares.h"
#include <bitset>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

class Utils {
  private:
    static void showReversedBin(int8_t);

  public:
    static constexpr uint8_t ROW_NUMBER = 8;
    static constexpr uint8_t COLUMN_NUMBER = 8;

    static constexpr uint8_t NUMBER_SQUARES_TABLE = ROW_NUMBER * COLUMN_NUMBER;

    static constexpr uint64_t A_FILE = 0x0101010101010101;
    static constexpr uint64_t H_FILE = 0x8080808080808080;
    static constexpr uint64_t FIRST_ROW = 0x00000000000000ff;
    static constexpr uint64_t LAST_ROW = 0xff00000000000000;

    static constexpr uint64_t BIT_SQUARE_LOOKUP_MAGIC = 0x03f79d71b4cb0a89;

    static constexpr uint8_t BIT_SQUARE_LOOKUP[64] = {
        0,  47, 1,  56, 48, 27, 2,  60, 57, 49, 41, 37, 28, 16, 3,  61, 54, 58, 35, 52, 50, 42, 21, 44, 38, 32, 29, 23, 17, 11, 4, 62,
        46, 55, 26, 59, 40, 36, 15, 53, 34, 51, 20, 43, 31, 22, 10, 45, 25, 39, 14, 33, 19, 30, 9,  24, 13, 18, 8,  12, 7,  6,  5, 63};

  private:
    static constexpr Square bitToSquare(const uint64_t &board) {
        return Square(BIT_SQUARE_LOOKUP[BIT_SQUARE_LOOKUP_MAGIC * (board ^ (board - 1)) >> 58]);
    }

  public:
    static Square popLSB(uint64_t &board) {
        const Square poppedBit = Utils::bitToSquare(board);
        board &= board - 1;
        return poppedBit;
    }

    static void runForEachSetBit(uint64_t bitboard, const std::function<void(const Square &)> &callEachBit) {
        while (bitboard) {
            const Square setSquare = Utils::popLSB(bitboard);
            callEachBit(setSquare);
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

    /**
     * @param[in] value bitboard
     * @param[in] value in rage from 0 to 7
     */
    static uint64_t getIthRank(const uint64_t &, const int &);
};

#endif // CHESS_ENGINE_UTILS_H
