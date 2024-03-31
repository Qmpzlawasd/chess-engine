#include "Board/LegalMove.h"


int main() {

        const std::string fenString = "7k/8/8/n4p2/3nb2K/2rP4/P1P2P2/8 w - - 0 1";
        Board board{fenString};
        LegalMove legalMove{board};

    //    std::vector<Move> movesKnight = legalMove.getKnightLegalMoves<WHITE>();
    //    std::for_each(movesKnight.begin(), movesKnight.end(), [](const Move &move) -> void { std::cout << move; });

    //    std::vector<Move> movesKing = legalMove.getKnightLegalMoves<WHITE>();
    //    std::for_each(movesKing.begin(), movesKing.end(), [](const Move &move) -> void { std::cout << move; });

        std::vector<Move> movesPawn = legalMove.getPawnLegalMoves<WHITE>();
        std::for_each(movesPawn.begin(), movesPawn.end(), [](const Move &move) -> void { std::cout << move; });
}
