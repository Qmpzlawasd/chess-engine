#include <vector>
#include <bitset>
#include <thread>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "MagicBoardGenerator.cpp"
#include "MagicBitboard.cpp"
#include "MagicValues.cpp"
#include "Squares.cpp"
#include "Piece.cpp"
#include "Utils.cpp"
#include "Board.cpp"

int main()
{

    const std::string asd  = "R7/8/8/8/8/8/8/8 w - - 0 1";
    Board board{asd};
    Utils::showBitBoard(board.getRookMoves(F4));
}
