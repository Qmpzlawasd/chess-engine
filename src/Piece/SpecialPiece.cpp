#include "SpecialPiece.h"

template <Color side>
[[nodiscard]] uint64_t Pawn::getMoves(const Square &square, const uint64_t &emptySquares) noexcept {
    const uint64_t pieceBitboard = Utils::setSquare(square);

    if constexpr (side == WHITE) {
        constexpr uint64_t THIRD_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER << Utils::ROW_NUMBER;

        const uint64_t basicAttack = (pieceBitboard << Utils::ROW_NUMBER) & emptySquares;
        const uint64_t setDoublePush = (basicAttack & THIRD_ROW) << Utils::ROW_NUMBER & emptySquares;

        return basicAttack ^ setDoublePush;
    } else {
        // move orientations are reversed
        constexpr uint64_t THIRD_TO_LAST_ROW = Utils::FIRST_ROW << (Utils::COLUMN_NUMBER * (Utils::COLUMN_NUMBER - 3));

        const uint64_t basicAttack = (pieceBitboard >> Utils::ROW_NUMBER) & emptySquares;
        const uint64_t setDoublePush = (basicAttack & THIRD_TO_LAST_ROW) >> Utils::ROW_NUMBER & emptySquares;

        return basicAttack ^ setDoublePush;
    }
}

template uint64_t Pawn::getMoves<BLACK>(const Square &, const uint64_t &) noexcept;
template uint64_t Pawn::getMoves<WHITE>(const Square &, const uint64_t &) noexcept;

template <Color side>
uint64_t Pawn::getThreatens(const Square &square) noexcept {
    const uint64_t pieceBitboard = Utils::setSquare(square);

    uint64_t attackLeft, attackRight;
    if constexpr (side == WHITE) {
        attackLeft = ((pieceBitboard & ~Utils::A_FILE) << (Utils::ROW_NUMBER - 1));
        attackRight = ((pieceBitboard & ~Utils::H_FILE) << (Utils::ROW_NUMBER + 1));
    } else {
        // move orientations are reversed
        attackLeft = ((pieceBitboard & ~Utils::H_FILE) >> (Utils::ROW_NUMBER - 1));
        attackRight = ((pieceBitboard & ~Utils::A_FILE) >> (Utils::ROW_NUMBER + 1));
    }

    return attackLeft | attackRight;
}

template uint64_t Pawn::getThreatens<WHITE>(const Square &square) noexcept;
template uint64_t Pawn::getThreatens<BLACK>(const Square &square) noexcept;
