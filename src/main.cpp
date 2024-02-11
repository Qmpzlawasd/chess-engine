#include "Board.h"
#include "MagicBitboard.h"
#include "Utils.h"
#include <bitset>
int main() {

    const std::string asd = "1R2Q3/R7/8/8/6Q1/2R5/Bn3R1R/1N1R4 w - - 0 1";
    Board board{asd};
    Utils::showBitBoard(board.getKnightMoves<Utils::BLACK>(F5));
}
