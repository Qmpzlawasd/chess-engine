#include "Piece.h"

uint64_t Rook::getNaiveAttackPattern(const Square &square) const noexcept {
    const uint8_t line = square % Utils::ROW_NUMBER;
    const uint8_t column = square / Utils::COLUMN_NUMBER;

    const uint64_t PIECE_FILE = Utils::A_FILE << line;
    const uint64_t PIECE_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * column;

    return (PIECE_FILE | PIECE_ROW) & ~Utils::setSquare(square);
}

uint64_t Bishop::getNaiveAttackPattern(const Square &square) const noexcept {
    const uint8_t column = square % Utils::ROW_NUMBER;
    const uint8_t line = square / Utils::COLUMN_NUMBER;

    uint64_t naiveAttackPattern = 0;

    for (int i = line, j = column; i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0; i++, j++) {
        naiveAttackPattern |= Utils::setSquare(Square(i * Utils::COLUMN_NUMBER + j));
    }
    for (int i = line, j = column; i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0; i++, j--) {
        naiveAttackPattern |= Utils::setSquare(Square(i * Utils::COLUMN_NUMBER + j));
    }
    for (int i = line, j = column; i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0; i--, j++) {
        naiveAttackPattern |= Utils::setSquare(Square(i * Utils::COLUMN_NUMBER + j));
    }
    for (int i = line, j = column; i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0; i--, j--) {
        naiveAttackPattern |= Utils::setSquare(Square(i * Utils::COLUMN_NUMBER + j));
    }
    return naiveAttackPattern & ~Utils::setSquare(square);
}

uint64_t Bishop::fillPositions(const Square &square, const uint64_t &pattern) const noexcept {
    const uint8_t column = square % Utils::ROW_NUMBER;
    const uint8_t line = square / Utils::COLUMN_NUMBER;

    uint64_t filledPattern = 0;

    for (int i = line, j = column; i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0; i++, j++) {
        uint64_t nextSquareSet = Utils::setSquare(Square(i * Utils::COLUMN_NUMBER + j));
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & pattern) != 0) {
            break;
        }
    }
    for (int i = line, j = column; i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0; i++, j--) {
        uint64_t nextSquareSet = Utils::setSquare(Square(i * Utils::COLUMN_NUMBER + j));
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & pattern) != 0) {
            break;
        }
    }
    for (int i = line, j = column; i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0; i--, j++) {
        uint64_t nextSquareSet = Utils::setSquare(Square(i * Utils::COLUMN_NUMBER + j));
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & pattern) != 0) {
            break;
        }
    }
    for (int i = line, j = column; i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0; i--, j--) {
        uint64_t nextSquareSet = Utils::setSquare(Square(i * Utils::COLUMN_NUMBER + j));
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & pattern) != 0) {
            break;
        }
    }
    return filledPattern & ~Utils::setSquare(square);
}

uint64_t Rook::fillPositions(const Square &square, const uint64_t &pattern) const noexcept {
    const uint8_t column = square % Utils::ROW_NUMBER;
    const uint8_t line = square / Utils::COLUMN_NUMBER;

    uint64_t filledPattern = 0;
    for (int i = column + 1; i < Utils::COLUMN_NUMBER; ++i) {
        const uint64_t nextSquareSet = (uint64_t)1 << (line * Utils::COLUMN_NUMBER + i);
        filledPattern |= nextSquareSet;

        if ((nextSquareSet & pattern) != 0) {
            break;
        }
    }

    for (int i = column - 1; i >= 0; --i) {
        const uint64_t nextSquareSet = (uint64_t)1 << (line * Utils::COLUMN_NUMBER + i);
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & pattern) != 0) {
            break;
        }
    }

    for (int i = line + 1; i < Utils::ROW_NUMBER; ++i) {
        const uint64_t nextSquareSet = (uint64_t)1 << (i * Utils::COLUMN_NUMBER + column);
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & pattern) != 0) {
            break;
        }
    }

    for (int i = line - 1; i >= 0; --i) {
        const uint64_t nextSquareSet = (uint64_t)1 << (i * Utils::COLUMN_NUMBER + column);
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & pattern) != 0) {
            break;
        }
    }

    return filledPattern;
}

uint64_t Queen::fillPositions(const Square &square, const uint64_t &pattern) const noexcept {
    return Rook{}.fillPositions(square, pattern) | Bishop{}.fillPositions(square, pattern);
}
uint64_t Queen::getNaiveAttackPattern(const Square &square) const noexcept {
    return Rook{}.getNaiveAttackPattern(square) | Bishop{}.getNaiveAttackPattern(square);
}
