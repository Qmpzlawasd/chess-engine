#include "Board.h"
#include "Piece/SpecialPiece.h"
#include "Utils.h"
#include <bitset>

int main() {

    const std::string asd = "8/3N4/2b1B1p1/1B2k3/rR1rR1n1/2R3K1/5B2/1r2n3 w - - 0 1";
    Board board{asd};
    Utils::showBitBoard(board.getDangerTable<BLACK>());
    Utils::showBitBoard(board.isSquareAttacked<WHITE>(E3));

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
