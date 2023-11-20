#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <bitset>
#include <unordered_map>
#include "Utils.cpp"
#include "MagicBitboard.cpp"
#include "Piece.cpp"
#include "Board.cpp"

int main()
{

    std::string asd  = "R7/8/8/8/3R4/8/8/7R w - - 0 1";
    Board board{asd};
    std::cout<<sizeof(uint32_t);
//    Utils::showBitBoard(board.getRookMoves(F4));
}
