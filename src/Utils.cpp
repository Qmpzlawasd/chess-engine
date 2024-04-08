#include "Utils.h"
#include "Constants.h"

void Utils::showReversedBin(int8_t nr) {
    for (size_t i = 0; i < Utils::ROW_NUMBER; i++) {
        std::cout << ((nr >> i) & 1) << "=";
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

uint64_t Utils::getSetLineBetween(const Square &square, const Square &square1) {
    return LINES_BETWEEN[static_cast<uint16_t>(square * 64 + square1)];
}

std::string_view Utils::squareToString(const Square &square) noexcept {

    switch (square) {
    case A1:
        return "a1";
    case B1:
        return "b1";
    case C1:
        return "c1";
    case D1:
        return "d1";
    case E1:
        return "e1";
    case F1:
        return "f1";
    case G1:
        return "g1";
    case H1:
        return "h1";
    case A2:
        return "a2";
    case B2:
        return "b2";
    case C2:
        return "c2";
    case D2:
        return "d2";
    case E2:
        return "e2";
    case F2:
        return "f2";
    case G2:
        return "g2";
    case H2:
        return "h2";
    case A3:
        return "a3";
    case B3:
        return "b3";
    case C3:
        return "c3";
    case D3:
        return "d3";
    case E3:
        return "e3";
    case F3:
        return "f3";
    case G3:
        return "g3";
    case H3:
        return "h3";
    case A4:
        return "a4";
    case B4:
        return "b4";
    case C4:
        return "c4";
    case D4:
        return "d4";
    case E4:
        return "e4";
    case F4:
        return "f4";
    case G4:
        return "g4";
    case H4:
        return "h4";
    case A5:
        return "a5";
    case B5:
        return "b5";
    case C5:
        return "c5";
    case D5:
        return "d5";
    case E5:
        return "e5";
    case F5:
        return "f5";
    case G5:
        return "g5";
    case H5:
        return "h5";
    case A6:
        return "a6";
    case B6:
        return "b6";
    case C6:
        return "c6";
    case D6:
        return "d6";
    case E6:
        return "e6";
    case F6:
        return "f6";
    case G6:
        return "g6";
    case H6:
        return "h6";
    case A7:
        return "a7";
    case B7:
        return "b7";
    case C7:
        return "c7";
    case D7:
        return "d7";
    case E7:
        return "e7";
    case F7:
        return "f7";
    case G7:
        return "g7";
    case H7:
        return "h7";
    case A8:
        return "a8";
    case B8:
        return "b8";
    case C8:
        return "c8";
    case D8:
        return "d8";
    case E8:
        return "e8";
    case F8:
        return "f8";
    case G8:
        return "g8";
    case H8:
        return "h8";
    default:
        return "Square not found";
    }
}
