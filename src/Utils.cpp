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
        return "A1";
    case B1:
        return "B1";
    case C1:
        return "C1";
    case D1:
        return "D1";
    case E1:
        return "E1";
    case F1:
        return "F1";
    case G1:
        return "G1";
    case H1:
        return "H1";
    case A2:
        return "A2";
    case B2:
        return "B2";
    case C2:
        return "C2";
    case D2:
        return "D2";
    case E2:
        return "E2";
    case F2:
        return "F2";
    case G2:
        return "G2";
    case H2:
        return "H2";
    case A3:
        return "A3";
    case B3:
        return "B3";
    case C3:
        return "C3";
    case D3:
        return "D3";
    case E3:
        return "E3";
    case F3:
        return "F3";
    case G3:
        return "G3";
    case H3:
        return "H3";
    case A4:
        return "A4";
    case B4:
        return "B4";
    case C4:
        return "C4";
    case D4:
        return "D4";
    case E4:
        return "E4";
    case F4:
        return "F4";
    case G4:
        return "G4";
    case H4:
        return "H4";
    case A5:
        return "A5";
    case B5:
        return "B5";
    case C5:
        return "C5";
    case D5:
        return "D5";
    case E5:
        return "E5";
    case F5:
        return "F5";
    case G5:
        return "G5";
    case H5:
        return "H5";
    case A6:
        return "A6";
    case B6:
        return "B6";
    case C6:
        return "C6";
    case D6:
        return "D6";
    case E6:
        return "E6";
    case F6:
        return "F6";
    case G6:
        return "G6";
    case H6:
        return "H6";
    case A7:
        return "A7";
    case B7:
        return "B7";
    case C7:
        return "C7";
    case D7:
        return "D7";
    case E7:
        return "E7";
    case F7:
        return "F7";
    case G7:
        return "G7";
    case H7:
        return "H7";
    case A8:
        return "A8";
    case B8:
        return "B8";
    case C8:
        return "C8";
    case D8:
        return "D8";
    case E8:
        return "E8";
    case F8:
        return "F8";
    case G8:
        return "G8";
    case H8:
        return "H8";
    default:
        return "Square not found";
    }
}
