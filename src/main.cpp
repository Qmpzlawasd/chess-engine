#include "Board/LegalMove.h"
#include "Pertf.h"

int main() {
    // idea when bishop is moved, restet only the danger tables for that sqare
    // draw no material

    Pertf::runTests();

    //    const std::string fenString = "rnbqkbnr/ppppppp1/8/7p/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1";
    //    Board board{fenString};
    //    LegalMove legalMove{board};
    //
    //    std::optional<std::vector<std::shared_ptr<Move>>> moves = legalMove.getLegalMoves<WHITE>();
    //    if (moves->empty()) {
    //        return 0;
    //    }
    //    std::cout << moves->size();
    //    std::for_each(moves->begin(), moves->end(), [](const std::shared_ptr<Move> &move) -> void { std::cout << *move; });
    //    moves->at(0)->makeMove(board);
}
