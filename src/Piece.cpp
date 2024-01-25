#include "Piece.h"

uint64_t Rook::getNaiveAttackPattern(const Square &square) {
    const uint8_t line = square % Utils::ROW_NUMBER;
    const uint8_t column = square / Utils::COLUMN_NUMBER;

    const uint64_t PIECE_FILE = Utils::A_FILE << line;
    const uint64_t PIECE_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * column;

    return (PIECE_FILE | PIECE_ROW) & ~Utils::setSquare(square);
}

uint64_t Bishop::getNaiveAttackPattern(const Square &square) {
    const uint8_t line = square % Utils::ROW_NUMBER;
    const uint8_t column = square / Utils::COLUMN_NUMBER;

    uint64_t naiveAttackPattern = 0;

    for (uint8_t i = line, j = column;
         i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0;
         i++, j++) {
        naiveAttackPattern |= Utils::setSquare(Square(j * Utils::COLUMN_NUMBER + i));
    }
    for (uint8_t i = line, j = column;
         i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0;
         i++, j--) {
        naiveAttackPattern |= Utils::setSquare(Square(j * Utils::COLUMN_NUMBER + i));
    }
    for (uint8_t i = line, j = column;
         i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0;
         i--, j++) {
        naiveAttackPattern |= Utils::setSquare(Square(j * Utils::COLUMN_NUMBER + i));
    }
    for (uint8_t i = line, j = column;
         i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0;
         i--, j--) {
        naiveAttackPattern |= Utils::setSquare(Square(j * Utils::COLUMN_NUMBER + i));
    }
    return naiveAttackPattern & ~Utils::setSquare(square);
}
