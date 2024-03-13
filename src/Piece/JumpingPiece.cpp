#include "JumpingPiece.h"

[[nodiscard]] uint64_t King::getMoves(const Square &square )  noexcept {
    const uint64_t pieceBitboard = Utils::setSquare(square);

    const uint64_t attackLeft = (pieceBitboard & ~Utils::A_FILE) >> 1;
    const uint64_t attackRight = (pieceBitboard & ~Utils::H_FILE) << 1;

    const uint64_t attackUp = (pieceBitboard >> Utils::ROW_NUMBER);
    const uint64_t attackDown = (pieceBitboard << Utils::ROW_NUMBER);

    const uint64_t attackLeftUp = (pieceBitboard & ~Utils::A_FILE) << (Utils::ROW_NUMBER - 1);
    const uint64_t attackLeftDown = (pieceBitboard & ~Utils::A_FILE) >> (Utils::ROW_NUMBER + 1);
    const uint64_t attackRightUp = (pieceBitboard & ~Utils::H_FILE) << (Utils::ROW_NUMBER + 1);
    const uint64_t attackRightDown = (pieceBitboard & ~Utils::H_FILE) >> (Utils::ROW_NUMBER - 1);

    return (attackLeftUp | attackLeftDown | attackRightUp | attackRightDown | attackLeft | attackRight | attackUp | attackDown) ;
}

[[nodiscard]] uint64_t Knight::getMoves(const Square &square) noexcept {
    const uint64_t pieceBitboard = Utils::setSquare(square);

    constexpr uint64_t B_FILE = Utils::A_FILE << 1;
    constexpr uint64_t G_FILE = Utils::H_FILE >> 1;

    constexpr uint64_t SECOND_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * 1;
    constexpr uint64_t EIGHTH_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * (Utils::ROW_NUMBER - 1);
    constexpr uint64_t SEVENTH_ROW = EIGHTH_ROW >> 1;

    const uint64_t attackLeftUpUp = (pieceBitboard & ~(SEVENTH_ROW | EIGHTH_ROW | Utils::A_FILE)) << (Utils::ROW_NUMBER * 2 - 1);
    const uint64_t attackLeftUpLeft = (pieceBitboard & ~(Utils::A_FILE | B_FILE | EIGHTH_ROW)) << (Utils::ROW_NUMBER - 2);
    const uint64_t attackLeftDownDown = (pieceBitboard & ~(Utils::FIRST_ROW | SECOND_ROW | Utils::A_FILE)) >> (Utils::ROW_NUMBER * 2 + 1);
    const uint64_t attackLeftDownLeft = (pieceBitboard & ~(Utils::A_FILE | Utils::FIRST_ROW | B_FILE)) >> (Utils::ROW_NUMBER + 2);

    const uint64_t attackRightUpUp = (pieceBitboard & ~(SEVENTH_ROW | EIGHTH_ROW | Utils::H_FILE)) << (Utils::ROW_NUMBER * 2 + 1);
    const uint64_t attackRightUpRight = (pieceBitboard & ~(Utils::H_FILE | G_FILE | EIGHTH_ROW)) << (Utils::ROW_NUMBER + 2);

    const uint64_t attackRightDownDown = (pieceBitboard & ~(Utils::FIRST_ROW | SECOND_ROW | Utils::H_FILE)) >> (Utils::ROW_NUMBER * 2 - 1);
    const uint64_t attackRightDownRight = (pieceBitboard & ~(Utils::H_FILE | Utils::FIRST_ROW | G_FILE)) >> (Utils::ROW_NUMBER - 2);

    return attackLeftUpUp | attackLeftUpLeft | attackLeftDownDown | attackLeftDownLeft | attackRightUpUp | attackRightUpRight |
           attackRightDownDown | attackRightDownRight;
}
