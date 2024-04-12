#ifndef CHESS_ENGINE_KNIGHTMOVE_H
#define CHESS_ENGINE_KNIGHTMOVE_H

#include "Move.h"

class KnightMove : public Move {
  public:
    explicit KnightMove(const Move &_move) noexcept : Move(_move){};

    void makeMove(Board &board) noexcept override {
        const auto fromSquare = static_cast<const Square>(this->getFrom());
        const auto toSquare = static_cast<const Square>(this->getTo());

        if (board.turn == WHITE) {
            Move::makeCapture<WHITE>(board, toSquare);
            board.knights.flipSquare<WHITE>(fromSquare);
            board.knights.flipSquare<WHITE>(toSquare);

        } else {
            Move::makeCapture<BLACK>(board, toSquare);
            board.knights.flipSquare<BLACK>(fromSquare);
            board.knights.flipSquare<BLACK>(toSquare);
        }

        board.resetEnPassant();
        Move::makeMove(board);
    }
};

#endif // CHESS_ENGINE_KNIGHTMOVE_H
