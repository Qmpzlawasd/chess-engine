#ifndef CHESS_ENGINE_BISHOPMOVE_H
#define CHESS_ENGINE_BISHOPMOVE_H

#include "Move.h"

class BishopMove : public Move {
    virtual void makeMove(Board &board) noexcept override {
        const Square fromSquare = static_cast<const Square>(this->getFrom());
        const Square toSquare = static_cast<const Square>(this->getTo());
        if (board.turn == WHITE) {
            board.bishops.flipSquare<WHITE>(fromSquare);
            board.bishops.flipSquare<WHITE>(toSquare);

        } else {
            board.bishops.flipSquare<BLACK>(fromSquare);
            board.bishops.flipSquare<BLACK>(toSquare);
        }

        board.resetEnPassant();
        Move::makeMove(board);
    }
};

#endif // CHESS_ENGINE_BISHOPMOVE_H
