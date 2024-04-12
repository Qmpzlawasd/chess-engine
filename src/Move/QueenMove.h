#ifndef CHESS_ENGINE_QUEENMOVE_H
#define CHESS_ENGINE_QUEENMOVE_H

#include "Move.h"

class QueenMove : public Move {
  public:
    explicit QueenMove(const Move &_move) noexcept : Move(_move){};

    void makeMove(Board &board) noexcept override {
        const auto fromSquare = static_cast<const Square>(this->getFrom());
        const auto toSquare = static_cast<const Square>(this->getTo());

        if (board.turn == WHITE) {
            Move::makeCapture<WHITE>(board, toSquare);

            board.queens.flipSquare<WHITE>(fromSquare);
            board.queens.flipSquare<WHITE>(toSquare);

        } else {
            Move::makeCapture<BLACK>(board, toSquare);

            board.queens.flipSquare<BLACK>(fromSquare);
            board.queens.flipSquare<BLACK>(toSquare);
        }

        board.resetEnPassant();
        Move::makeMove(board);
    }
};

#endif // CHESS_ENGINE_QUEENMOVE_H
