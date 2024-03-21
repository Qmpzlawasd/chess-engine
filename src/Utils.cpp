#include "Utils.h"
#include "Constants.h"

void Utils::showReversedBin(int8_t nr) {
    for (size_t i = 0; i < Utils::ROW_NUMBER; i++) {
        std::cout << ((nr >> i) & 1) << "-";
    }
    std::cout << "\n";
}

void Utils::showBitBoard(const Square &square) { showBitBoard(Utils::setSquare(square)); }

void Utils::showBitBoard(const uint64_t &board) {
    for (int8_t i = ROW_NUMBER - 1; i >= 0; i--) {
        showReversedBin((board >> (i * ROW_NUMBER)) & (static_cast<unsigned int>(1 << ROW_NUMBER) - 1));
    }
    printf("\n");
}

/**
 * @param[in] value bitboard
 * @param[in] value in rage from 0 to 7
 */
uint64_t Utils::getIthFile(const uint64_t &table, const int &I) { return table & A_FILE >> I; }

uint64_t Utils::setSquare(const Square &square) { return static_cast<uint64_t>(1) << square; }
uint64_t Utils::setSquare(const std::vector<Square> &squares) {
    uint64_t setBoard = 0;
    std::for_each(squares.begin(), squares.end(), [&setBoard](const Square &square) -> void { setBoard |= Utils::setSquare(square); });
    return setBoard;
}

/**
 * @param[in] value bitboard
 * @param[in] value in rage from 0 to 7
 */
uint64_t Utils::getIthRank(const uint64_t &table, const int &I) { return table & FIRST_ROW << I * Utils::ROW_NUMBER; }

uint64_t Utils::getSetLineBetween(const Square &square, const Square& square1) {
    return LINES_BETWEEN[square * 64 + square1];

}
