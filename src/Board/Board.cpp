#include "Board.h"

template <Color side>
uint64_t Board::getOccupiedSquares() const {
    return king.getBitboard<side>() | queens.getBitboard<side>() | rooks.getBitboard<side>() | bishops.getBitboard<side>() |
           knights.getBitboard<side>() | pawns.getBitboard<side>();
}

uint64_t Board::getEmptySquares() const { return ~(getOccupiedSquares<WHITE>() | getOccupiedSquares<BLACK>()); }

void Board::printBoard(std::ostream &os) const {}

void Board::printStatus(std::ostream &os) const {
    //    os << "Move " << fullmoveNumber << ": " << (castle[1] ? "Q" : "") << (castle[3] ? "q" : "") << "\t" << (int)halfmoveClock << "\t"
    //       << enPassant << "\t" << (castle[0] ? "K" : "") << (castle[2] ? "k" : "") << "\n";
}
