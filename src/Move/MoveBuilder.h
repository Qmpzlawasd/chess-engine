#ifndef CHESS_ENGINE_MOVEBUILDER_H
#define CHESS_ENGINE_MOVEBUILDER_H

#include "Enums/Squares.h"
#include "Move.h"

class MoveBuilder {
    Move move;


  public:
    explicit MoveBuilder() : move(0) {};

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
