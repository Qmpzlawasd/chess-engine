#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <bitset>
#include <unordered_map>
#include "Squares.cpp"
#include "Utils.cpp"
#include "MagicBitboard.cpp"
#include "Piece.cpp"
#include "Board.cpp"
#include <thread>
int main()
{
    const std::string asd  = "R7/8/8/8/8/8/8/8 w - - 0 1";
    Board board{asd};
    Utils::showBitBoard(board.getRookMoves(F4));
}
