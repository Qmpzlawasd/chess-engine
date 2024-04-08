#ifndef CHESS_ENGINE_BISHOPMOVE_H
#define CHESS_ENGINE_BISHOPMOVE_H

#include "Move.h"

class BishopMove : public Move {
  public:
    BishopMove(const Move &move) : Move(move){};

    virtual void makeMove(Board &board) noexcept override {
        const auto fromSquare = static_cast<const Square>(this->getFrom());
        const auto toSquare = static_cast<const Square>(this->getTo());

        if (board.turn == WHITE) {
            Move::makeCapture<WHITE>(board, toSquare);

            board.bishops.flipSquare<WHITE>(fromSquare);
            board.bishops.flipSquare<WHITE>(toSquare);

        } else {
            Move::makeCapture<BLACK>(board, toSquare);

            board.bishops.flipSquare<BLACK>(fromSquare);
            board.bishops.flipSquare<BLACK>(toSquare);
        }

        board.resetEnPassant();
        Move::makeMove(board);
    }
};

#endif // CHESS_ENGINE_BISHOPMOVE_H
