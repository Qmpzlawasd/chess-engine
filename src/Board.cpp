#include "Board.h"

[[nodiscard]] uint64_t Board::getFullWhiteSquares() const {
    return king.getWhite() | queens.getWhite() | rooks.getWhite() | bishops.getWhite() | knights.getWhite() | pawns.getWhite();
}

[[nodiscard]] uint64_t Board::getFullBlackSquares() const {
    return king.getBlack() | queens.getBlack() | rooks.getBlack() | bishops.getBlack() | knights.getBlack() | pawns.getBlack();
}

[[nodiscard]] uint64_t Board::getEmptySquares() const { return ~(getFullBlackSquares() | getFullWhiteSquares()); }

void Board::printBoard(std::ostream &os) const {}

void Board::printStatus(std::ostream &os) const {
    os << "Move " << fullmoveNumber << ": " << (castle[1] ? "Q" : "") << (castle[3] ? "q" : "") << "\t" << (int)halfmoveClock << "\t"
       << enPassant << "\t" << (castle[0] ? "K" : "") << (castle[2] ? "k" : "") << "\n";
}
