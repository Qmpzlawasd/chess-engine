#include "Board.cpp"
#include "MagicBitboard.cpp"
#include "MagicBoardGenerator.cpp"
#include "MagicValues.cpp"
#include "Piece.cpp"
#include "Squares.cpp"
#include "Utils.cpp"
#include <bitset>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>

int main() {

    const std::string asd = "4n3/8/8/8/1Q2R2Q/8/4n3/8 w - - 0 1";    Board board{asd};
    Utils::showBitBoard(board.getBishopMoves(E4));
}
