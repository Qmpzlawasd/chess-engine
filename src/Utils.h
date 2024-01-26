//
// Created by stefan on 25/01/24.
//

#ifndef CHESS_ENGINE_UTILS_H
#define CHESS_ENGINE_UTILS_H

#include "Squares.h"
#include <bitset>
#include <iostream>
#include <thread>
#include <vector>

class Utils {
  private:
    static void showReversedBin(int8_t nr);

  public:
    static constexpr bool WHITE = true;
    static constexpr bool BLACK = false;

    static constexpr uint8_t ROW_NUMBER = 8;
    static constexpr uint8_t COLUMN_NUMBER = 8;

    static constexpr uint8_t NUMBER_SQUARES_TABLE = ROW_NUMBER * COLUMN_NUMBER;

    static constexpr uint64_t A_FILE = 0x0101010101010101;
    static constexpr uint64_t H_FILE = 0x8080808080808080;
    static constexpr uint64_t FIRST_ROW = 0x00000000000000ff;

    static void showBitBoard(const uint64_t &board);

    /**
     * @param[in] value bitboard
     * @param[in] value in rage from 0 to 7
     */
    static uint64_t getIthFile(const uint64_t &table, const int &I);

    /**
     * @param[in] value bitboard
     * @param[in] value in rage from 0 to 7
     */
    static uint64_t setSquare(const Square &square);

    /**
     * @param[in] value bitboard
     * @param[in] value in rage from 0 to 7
     */
    static uint64_t getIthRank(const uint64_t &table, const int &I);
};

#endif // CHESS_ENGINE_UTILS_H
