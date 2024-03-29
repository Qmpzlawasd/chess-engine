#ifndef CHESS_ENGINE_LEGALMOVE_H
#define CHESS_ENGINE_LEGALMOVE_H

#include "../Piece/JumpingPiece.h"
#include "Board.h"
#include "Enums/Colors.h"

#include <cstdint>

class LegalMove {
    Board board;

  public:
    explicit LegalMove(const Board &_board) noexcept : board{_board} {}

    template <Color side>
    [[nodiscard]] std::vector<Move> getBishopLegalMoves() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getRookLegalMoves() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getPawnLegalMoves() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getKnightLegalMoves() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getQueenLegalMoves() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getKingLegalMoves() const noexcept;
};

#include "LegalMove.inl"

#endif // CHESS_ENGINE_LEGALMOVE_H
