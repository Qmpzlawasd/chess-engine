//#include "Board/Board.h"
#include "Board/LegalMove.h"

int main() {
    // implement pins
    const std::string asd = "q5b1/1R6/4P3/q1BK1B1r/8/8/2k5/6R1 w - - 0 1";
    Board board{asd};
    LegalMove legalMove{board};
    const uint64_t a = board.getPinMaskHV<WHITE>();
    const uint64_t b = board.getPinMaskD12<WHITE>();
    Utils::showBitBoard(a);
    Utils::showBitBoard(b);
//        std::for_each(legalMove.getKingLegalMove<WHITE>().begin(), legalMove.getKingLegalMove<WHITE>().end(), [](const Move &move) {
//            std::cout << move;
//        });

    //    Utils::showBitBoard(board.isSquareAttacked<WHITE>(E3));

    //    std::vector <Move> as= board.getMovesFromBitboard(G5, table);
    //    std::for_each(as.begin(), as.end(), [](const Move &m) {
    //        std::cout << m;
    //    });

    //        MoveBuilder a{};
    //        std::cout << a.toSquare(A1).fromSquare(A2).withKingSideCastle();
    //        std::cout << board.rookAttacksSquare<WHITE>(C4) << '\n';
    //    std::cout << board.pawnAttacksSquare<WHITE>(D3) << '\n';
    //    Utils::showBitBoard(board.rooks.getBlockedAttackPattern(C8 , ~board.getEmptySquares()));
    //    Utils::showBitBoard(board.getPinnedSquares<BLACK>());
    //    Utils::showBitBoard(Pawn::getMoves<BLACK>(C7,board.getEmptySquares() & Utils::setSquare(C7)));
}
