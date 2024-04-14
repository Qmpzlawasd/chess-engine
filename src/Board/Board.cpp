#include "Board.h"

uint64_t Board::getEmptySquares() const { return ~(getOccupiedSquares<WHITE>() | getOccupiedSquares<BLACK>()); }

void Board::printBoard(std::ostream &os) const {}

void Board::printStatus(std::ostream &os) const {}
