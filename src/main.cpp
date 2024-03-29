#include "Board/LegalMove.h"

int main() {

    const std::string fenString = "1k5n/2N5/b7/4N1r1/8/8/4K3/8 w - - 0 1";
    Board board{fenString};
    LegalMove legalMove{board};

    std::vector<Move> moves = legalMove.getKnightLegalMoves<WHITE>();
    std::for_each(moves.begin(), moves.end(), [](const Move &move) -> void { std::cout << move; });
}
