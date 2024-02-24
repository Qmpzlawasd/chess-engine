#include "Piece/Piece.h"

template <Color side>
[[nodiscard]] uint64_t Pawn::getMoves(const Square &square) noexcept {
    const uint64_t pieceBitboard = Utils::setSquare(square);

    uint64_t forwardMove, attackLeft, attackRight;
    if constexpr (side == WHITE) {
        forwardMove = pieceBitboard << Utils::ROW_NUMBER;
        attackLeft = ((pieceBitboard & ~Utils::A_FILE) << (Utils::ROW_NUMBER - 1));
        attackRight = ((pieceBitboard & ~Utils::H_FILE) << (Utils::ROW_NUMBER + 1));
    } else {
        // move orientations are reversed
        forwardMove = pieceBitboard >> Utils::ROW_NUMBER;
        attackLeft = ((pieceBitboard & ~Utils::H_FILE) >> (Utils::ROW_NUMBER - 1));
        attackRight = ((pieceBitboard & ~Utils::A_FILE) >> (Utils::ROW_NUMBER + 1));
    }

    return forwardMove | attackLeft | attackRight;
}

template uint64_t Pawn::getMoves<BLACK>(const Square &square) noexcept;
template uint64_t Pawn::getMoves<WHITE>(const Square &square) noexcept;