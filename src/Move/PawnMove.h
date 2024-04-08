#ifndef CHESS_ENGINE_PAWNMOVE_H
#define CHESS_ENGINE_PAWNMOVE_H

#include "Move.h"

class PawnMove : public Move {
  public:
    PawnMove(const Move &move) : Move(move){};

    virtual void makeMove(Board &board) noexcept override {
        board.resetHalfMoveClock();

        const auto fromSquare = static_cast<const Square>(this->getFrom());
        const auto toSquare = static_cast<const Square>(this->getTo());
        board.resetEnPassant();

        if (board.turn == WHITE) {
            if (this->isCapture()) {
                Move::makeCapture<WHITE>(board, toSquare);
            } else if (this->isDoublePush()) {
                board.setEnPassant(Utils::setSquare(fromSquare) << Utils::ROW_NUMBER);
            }

            board.pawns.flipSquare<WHITE>(fromSquare);
            board.pawns.flipSquare<WHITE>(toSquare);

        } else {
            if (this->isCapture()) {
                Move::makeCapture<BLACK>(board, toSquare);
            } else if (this->isDoublePush()) {
                board.setEnPassant(Utils::setSquare(fromSquare) << Utils::ROW_NUMBER);
            }

            board.pawns.flipSquare<BLACK>(fromSquare);
            board.pawns.flipSquare<BLACK>(toSquare);
        }

        Move::makeMove(board);
    }
};

#endif // CHESS_ENGINE_PAWNMOVE_H
