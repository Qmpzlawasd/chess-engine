#ifndef CHESS_ENGINE_KINGMOVE_H
#define CHESS_ENGINE_KINGMOVE_H

#include "Move.h"

class KingMove : public Move {
  public:
    KingMove(const Move &move) : Move(move){};

    virtual void makeMove(Board &board) noexcept override {
        const auto fromSquare = static_cast<const Square>(this->getFrom());
        const auto toSquare = static_cast<const Square>(this->getTo());

        if (board.turn == WHITE) {
            Move::makeCapture<WHITE>(board, toSquare);
            board.castleWhite.kingMoved();

            board.king.flipSquare<WHITE>(fromSquare);
            board.king.flipSquare<WHITE>(toSquare);
            if (isQueenSideCastle()) {
                board.rooks.flipSquare<WHITE>(A1);
                board.rooks.flipSquare<WHITE>(D1);
            } else if (isKingSideCastle()) {
                board.rooks.flipSquare<WHITE>(H1);
                board.rooks.flipSquare<WHITE>(F1);
            }

        } else {
            Move::makeCapture<BLACK>(board, toSquare);
            board.castleBlack.kingMoved();

            board.king.flipSquare<BLACK>(fromSquare);
            board.king.flipSquare<BLACK>(toSquare);
            if (isQueenSideCastle()) {
                board.rooks.flipSquare<BLACK>(A8);
                board.rooks.flipSquare<BLACK>(D8);
            } else if (isKingSideCastle()) {
                board.rooks.flipSquare<BLACK>(H8);
                board.rooks.flipSquare<BLACK>(F8);
            }
        }

        board.resetEnPassant();
        Move::makeMove(board);
    }
};

#endif // CHESS_ENGINE_KINGMOVE_H
