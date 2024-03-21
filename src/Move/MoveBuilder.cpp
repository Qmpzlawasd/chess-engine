#include "MoveBuilder.h"
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
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}
Move MoveBuilder::withKingSideCastle() {
    move.setKingSideCastle();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}
Move MoveBuilder::withQueenSideCastle() {
    move.setQueenSideCastle();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}
Move MoveBuilder::withEnPassantCapture() {
    move.setEnPassantCapture();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}
Move MoveBuilder::withEnPassant() {
    move.setEnPassant();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
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
Move MoveBuilder::getMove() {
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}
MoveBuilder::MoveBuilder() : move(0) {}
