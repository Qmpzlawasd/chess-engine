#ifndef CHESS_ENGINE_LEGALMOVE_H
#define CHESS_ENGINE_LEGALMOVE_H

#include "../Colors.h"
#include "../Piece/JumpingPiece.h"
#include "Board.h"
#include <cstdint>
class LegalMove {
    Board board;

  public:
    explicit LegalMove(const Board &board) noexcept : board{board} {}

    template <Color side>
    [[nodiscard]] uint64_t getBishopLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] uint64_t getRookLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] uint64_t getPawnLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] uint64_t getKnightLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] uint64_t getQueenLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] uint64_t getKingLegalMove() const noexcept;
};

#endif // CHESS_ENGINE_LEGALMOVE_H
