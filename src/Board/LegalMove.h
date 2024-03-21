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
    [[nodiscard]] std::vector<Move> getBishopLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getRookLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getPawnLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getKnightLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getQueenLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getKingLegalMove() const noexcept;
};

#endif // CHESS_ENGINE_LEGALMOVE_H
