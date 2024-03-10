#ifndef CHESS_ENGINE_MOVEBUILDER_H
#define CHESS_ENGINE_MOVEBUILDER_H

#include "Move.h"
#include "../Squares.h"

class MoveBuilder {
    Move move;

  public:
    MoveBuilder &toSquare(const Square &);

    MoveBuilder &fromSquare(const Square &);

    MoveBuilder &withPromotion();

    MoveBuilder &withCapture();

    MoveBuilder &withKnight();

    MoveBuilder &withBishop();

    MoveBuilder &withRook();

    MoveBuilder &withQueen();

    Move withDoublePawnPush();

    Move withKingSideCastle();

    Move withQueenSideCastle();

    Move withEnPassantCapture();

    Move withEnPassant();

    Move getMove();
};
#endif // CHESS_ENGINE_MOVEBUILDER_H
