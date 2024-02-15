#include "Board.h"
#include "Utils.h"
#include <bitset>
int main() {

    const std::string asd = "2NB1B2/B3r3/qr4N1/1rb5/6rn/1r2N3/2r3pk/3rN1RP w - - 0 1";
    Board board{asd};

    //    Utils::showBitBoard(board.getKnightMoves<Utils::BLACK>(F5));
    printf("%ld", board.getRookMoves<Utils::WHITE>(G5));
}
