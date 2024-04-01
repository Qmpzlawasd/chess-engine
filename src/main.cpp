#include "Board/LegalMove.h"

int main() {

    const std::string fenString = "8/1K6/8/3pP3/8/5qk1/3q4/8 w - d6 0 1";
    Board board{fenString};
    LegalMove legalMove{board};

    //        std::vector<Move> movesKnight = legalMove.getPawnLegalMoves<WHITE>();
    //        std::for_each(movesKnight.begin(), movesKnight.end(), [](const Move &move) -> void { std::cout << move; });

    //        std::vector<Move> movesKnight = legalMove.getBishopLegalMoves<WHITE>();
    //        std::for_each(movesKnight.begin(), movesKnight.end(), [](const Move &move) -> void { std::cout << move; });

    //        std::vector<Move> movesKnight = legalMove.getKnightLegalMoves<BLACK>();
    //        std::for_each(movesKnight.begin(), movesKnight.end(), [](const Move &move) -> void { std::cout << move; });

    //    std::vector<Move> movesKing = legalMove.getKnightLegalMoves<WHITE>();
    //    std::for_each(movesKing.begin(), movesKing.end(), [](const Move &move) -> void { std::cout << move; });

    //        std::vector<Move> movesPawn = legalMove.getPawnLegalMoves<WHITE>();
    //        std::for_each(movesPawn.begin(), movesPawn.end(), [](const Move &move) -> void { std::cout << move; });
}
