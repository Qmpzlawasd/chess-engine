#include "Utils.h"

void Utils::showReversedBin(int8_t nr) {
    for (size_t i = 0; i < Utils::ROW_NUMBER; i++) {
        std::cout << ((nr >> i) & 1) << "-";
    }
    std::cout << "\n";
}

void Utils::showBitBoard(const uint64_t &board) {
    for (int8_t i = ROW_NUMBER - 1; i >= 0; i--) {
        showReversedBin((board >> (i * ROW_NUMBER)) & ((unsigned int)(1 << ROW_NUMBER) - 1));
    }
    printf("\n");
}

/**
 * @param[in] value bitboard
 * @param[in] value in rage from 0 to 7
 */
uint64_t Utils::getIthFile(const uint64_t &table, const int &I) { return table & A_FILE >> I; }

uint64_t Utils::setSquare(const Square &square) { return (uint64_t)1 << square; }

/**
 * @param[in] value bitboard
 * @param[in] value in rage from 0 to 7
 */
uint64_t Utils::getIthRank(const uint64_t &table, const int &I) { return table & FIRST_ROW << I * Utils::ROW_NUMBER; }
