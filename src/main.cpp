#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <bitset>
#include "Utils.cpp"
#include "Piece.cpp"
#include "Board.cpp"

int main()
{

    std::string asd  = "N1qp3p/R1qp3q/k2b3N/P2R3N/4Q2b/7N/NK1QQ2N/NN1R2B1 w - - 0 1";
    Board board{asd};
    Utils::showBitBoard(board.getFullWhiteSquares());
    Utils::showBitBoard(board.getKightMoves(true));
}
