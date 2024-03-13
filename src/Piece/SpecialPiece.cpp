#include "SpecialPiece.h"

template <Color side>
[[nodiscard]] uint64_t Pawn::getMoves(const Square &square, const uint64_t &emptySquares) noexcept {
    const uint64_t pieceBitboard = Utils::setSquare(square);

    if constexpr (side == WHITE) {
        constexpr uint64_t SECOND_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER;
        return (pieceBitboard << Utils::ROW_NUMBER | (pieceBitboard & SECOND_ROW) << Utils::ROW_NUMBER * 2) & emptySquares;
    } else {
        // move orientations are reversed
        constexpr uint64_t SECOND_TO_LAST_ROW = Utils::FIRST_ROW << (Utils::COLUMN_NUMBER * (Utils::COLUMN_NUMBER - 2));
        return (pieceBitboard >> Utils::ROW_NUMBER | (pieceBitboard & SECOND_TO_LAST_ROW) >> Utils::ROW_NUMBER * 2) & emptySquares;
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
