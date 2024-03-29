#include "SlidingPiece.h"
#include "../MagicBitboard.h"

uint64_t Rook::getNaiveAttackPattern(const Square &square) const noexcept {
    const uint8_t line = square % Utils::ROW_NUMBER;
    const uint8_t column = square / Utils::COLUMN_NUMBER;

    const uint64_t PIECE_FILE = Utils::A_FILE << line;
    const uint64_t PIECE_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * column;

    return (PIECE_FILE | PIECE_ROW) & ~Utils::setSquare(square);
}

uint64_t Rook::getMoves(const Square &square, const uint64_t &emptySquares) noexcept {
    const uint64_t blockerPattern = Rook{}.getNaiveAttackPattern(square) & ~emptySquares;
    const uint64_t optimisedIndex = blockerPattern * Rook::MAGIC_CONSTANTS[square] >> Rook::SHIFT_VALUE;
    return magicBitboard.rookMoveTable[square][optimisedIndex];
}

uint64_t Rook::getThreatens(const Square &square, const uint64_t &allyPieces) noexcept {
    const uint64_t blockerPattern = Rook{}.getNaiveAttackPattern(square) & allyPieces;
    const uint64_t optimisedIndex = blockerPattern * Rook::MAGIC_CONSTANTS[square] >> Rook::SHIFT_VALUE;
    return magicBitboard.rookMoveTable[square][optimisedIndex];
}

uint64_t Rook::getBlockedAttackPattern(const Square &square, const uint64_t &blockerPattern) const noexcept {
    const uint8_t column = square % Utils::ROW_NUMBER;
    const uint8_t line = square / Utils::COLUMN_NUMBER;

    uint64_t filledPattern = 0;
    for (int i = column + 1; i < Utils::COLUMN_NUMBER; ++i) {
        const uint64_t nextSquareSet = static_cast<uint64_t>(1) << (line * Utils::COLUMN_NUMBER + i);
        filledPattern |= nextSquareSet;

        if ((nextSquareSet & blockerPattern) != 0) {
            break;
        }
    }

    for (int i = column - 1; i >= 0; --i) {
        const uint64_t nextSquareSet = static_cast<uint64_t>(1) << (line * Utils::COLUMN_NUMBER + i);
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & blockerPattern) != 0) {
            break;
        }
    }

    for (int i = line + 1; i < Utils::ROW_NUMBER; ++i) {
        const uint64_t nextSquareSet = static_cast<uint64_t>(1) << (i * Utils::COLUMN_NUMBER + column);
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & blockerPattern) != 0) {
            break;
        }
    }

    for (int i = line - 1; i >= 0; --i) {
        const uint64_t nextSquareSet = static_cast<uint64_t>(1) << (i * Utils::COLUMN_NUMBER + column);
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & blockerPattern) != 0) {
            break;
        }
    }

    return filledPattern;
}

// uint64_t Rook::getBlockedAttackPattern(const uint64_t &pieces, const uint64_t &pattern) const noexcept {
//     puts("DO NOT USE getBlockedAttackPattern");
//     constexpr uint64_t borderBitBoard = Utils::A_FILE | Utils::H_FILE;
//     uint64_t attackPiecesVertical = pieces;
//     uint64_t attackPiecesHorizontal = pieces;
//     for (int i = 0; i < Utils::COLUMN_NUMBER; ++i) {
//         attackPiecesHorizontal |= (attackPiecesHorizontal >> (Utils::ROW_NUMBER)) & ~pattern;
//     }
//     for (int i = 0; i < Utils::COLUMN_NUMBER; ++i) {
//         attackPiecesHorizontal |= (attackPiecesHorizontal << (Utils::ROW_NUMBER)) & ~pattern;
//     }
//
//     for (int i = 0; i < Utils::ROW_NUMBER; ++i) {
//         attackPiecesVertical |= (attackPiecesVertical >> 1) & (~pattern & ~borderBitBoard);
//     }
//     attackPiecesVertical |= (attackPiecesVertical >> 1) & ~pattern;
//     for (int i = 0; i < Utils::ROW_NUMBER; ++i) {
//         attackPiecesVertical |= (attackPiecesVertical << 1) & (~pattern & ~borderBitBoard);
//     }
//     attackPiecesVertical |= (attackPiecesVertical << 1) & ~pattern;
//     return (attackPiecesHorizontal | attackPiecesVertical) & ~pieces;
// }

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

uint64_t Bishop::getMoves(const Square &square, const uint64_t &emptySquares) noexcept {
    const uint64_t blockerPattern = Bishop{}.getNaiveAttackPattern(square) & ~emptySquares;
    const uint64_t optimisedIndex = blockerPattern * Bishop::MAGIC_CONSTANTS[square] >> Bishop::SHIFT_VALUE;
    return magicBitboard.bishopMoveTable[square][optimisedIndex];
}

uint64_t Bishop::getThreatens(const Square &square, const uint64_t &allyPieces) noexcept {
    const uint64_t blockerPattern = Bishop{}.getNaiveAttackPattern(square) & allyPieces;
    const uint64_t optimisedIndex = blockerPattern * Bishop::MAGIC_CONSTANTS[square] >> Bishop::SHIFT_VALUE;
    return magicBitboard.bishopMoveTable[square][optimisedIndex];
}

uint64_t Bishop::getBlockedAttackPattern(const Square &square, const uint64_t &blockerPattern) const noexcept {
    const uint8_t column = square % Utils::ROW_NUMBER;
    const uint8_t line = square / Utils::COLUMN_NUMBER;

    uint64_t filledPattern = 0;

    for (int i = line, j = column; i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0; i++, j++) {
        uint64_t nextSquareSet = Utils::setSquare(Square(i * Utils::COLUMN_NUMBER + j));
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & blockerPattern) != 0) {
            break;
        }
    }
    for (int i = line, j = column; i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0; i++, j--) {
        uint64_t nextSquareSet = Utils::setSquare(Square(i * Utils::COLUMN_NUMBER + j));
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & blockerPattern) != 0) {
            break;
        }
    }
    for (int i = line, j = column; i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0; i--, j++) {
        uint64_t nextSquareSet = Utils::setSquare(Square(i * Utils::COLUMN_NUMBER + j));
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & blockerPattern) != 0) {
            break;
        }
    }
    for (int i = line, j = column; i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0; i--, j--) {
        uint64_t nextSquareSet = Utils::setSquare(Square(i * Utils::COLUMN_NUMBER + j));
        filledPattern |= nextSquareSet;
        if ((nextSquareSet & blockerPattern) != 0) {
            break;
        }
    }
    return filledPattern & ~Utils::setSquare(square);
}

uint64_t Queen::getNaiveAttackPattern(const Square &square) const noexcept {
    return Rook{}.getNaiveAttackPattern(square) | Bishop{}.getNaiveAttackPattern(square);
}

uint64_t Queen::getMoves(const Square &square, const uint64_t &emptySquares) noexcept {
    return Rook::getMoves(square, emptySquares) | Bishop::getMoves(square, emptySquares);
}

uint64_t Queen::getThreatens(const Square &square, const uint64_t &allyPieces) noexcept {
    return Rook::getThreatens(square, allyPieces) | Bishop::getThreatens(square, allyPieces);
}

uint64_t Queen::getBlockedAttackPattern(const Square &square, const uint64_t &blockerPattern) const noexcept {
    return Rook{}.getBlockedAttackPattern(square, blockerPattern) | Bishop{}.getBlockedAttackPattern(square, blockerPattern);
}