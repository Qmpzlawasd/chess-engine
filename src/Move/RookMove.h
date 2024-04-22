#ifndef CHESS_ENGINE_ROOKMOVE_H
#define CHESS_ENGINE_ROOKMOVE_H

#include "Move.h"

class RookMove : public Move {
  public:
    explicit RookMove(const Move &_move) noexcept : Move(_move){};

    void makeMove(Board &board) noexcept override {
        const auto fromSquare = static_cast<const Square>(this->getFrom());
        const auto toSquare = static_cast<const Square>(this->getTo());

        //        zobristHashProps.toSquare = toSquare;
        //        zobristHashProps.fromSquare = fromSquare;

        if (board.turn == WHITE) {
            Move::makeCapture<WHITE>(board, toSquare);

            if (fromSquare == H1) {
                board.castleWhite.rookMoved<KING_SIDE>();
            } else if (fromSquare == A1) {
                board.castleWhite.rookMoved<QUEEN_SIDE>();
            }

            board.rooks.flipSquare<WHITE>(fromSquare);
            board.rooks.flipSquare<WHITE>(toSquare);

        } else {
            Move::makeCapture<BLACK>(board, toSquare);

            if (fromSquare == H8) {
                board.castleBlack.rookMoved<KING_SIDE>();
            } else if (fromSquare == A8) {
                board.castleBlack.rookMoved<QUEEN_SIDE>();
            }

            board.rooks.flipSquare<BLACK>(fromSquare);
            board.rooks.flipSquare<BLACK>(toSquare);
        }

        board.resetEnPassant();
        Move::makeMove(board);
    }
};

#endif // CHESS_ENGINE_ROOKMOVE_H
