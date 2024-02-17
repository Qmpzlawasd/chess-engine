//
// Created by stefan on 15/02/24.
//

#include "Move.h"
MoveBuilder &MoveBuilder::toSquare(const Square &toSquare) {
    move.setToSquare(toSquare);
    return *this;
}
MoveBuilder &MoveBuilder::fromSquare(const Square &fromSquare) {
    move.setFromSquare(fromSquare);
    return *this;
}
MoveBuilder &MoveBuilder::withPromotion() {
    move.setPromotion();
    return *this;
}
MoveBuilder &MoveBuilder::withCapture() {
    move.setCapture();
    return *this;
}
Move MoveBuilder::withDoublePawnPush() {
    move.setDoublePawnPush();
    return move;
}
Move MoveBuilder::withKingSideCastle() {
    move.setKingSideCastle();
    return move;
}
Move MoveBuilder::withQueenSideCastle() {
    move.setQueenSideCastle();
    return move;
}
Move MoveBuilder::withEnPassantCapture() {
    move.setEnPassantCapture();
    return move ;
}
Move MoveBuilder::withEnPassant() {
    move.setEnPassant();
    return move;
}
MoveBuilder &MoveBuilder::withKnight() {
    move.setWithKnight();
    return *this;
}
MoveBuilder &MoveBuilder::withBishop() {
    move.setWithBishop();
    return *this;
}
MoveBuilder &MoveBuilder::withRook() {
    move.setWithRook();
    return *this;
}
MoveBuilder &MoveBuilder::withQueen() {
    move.setWithQueen();
    return *this;
}
Move MoveBuilder::getMove() { return move; }