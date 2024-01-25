#include "Board.h"
#include "Utils.h"
#include <bitset>

int main() {

    const std::string asd = "4n3/8/8/8/1Q2R2Q/8/4n3/8 w - - 0 1";
    Board board{asd};
    Utils::showBitBoard(board.getBishopMoves(E4));
}
