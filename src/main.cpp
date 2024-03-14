#include "Board.h"
#include "Utils.h"
#include <bitset>

int main() {
        // add castles info
    const std::string asd = "8/1rk1P3/2B3P1/1B3N2/PrRK2Pp/4N1P1/1q3b2/3r3B w - - 0 1";
    Board board{asd};
    Utils::showBitBoard(board.getKingLegalMove<WHITE>());
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

