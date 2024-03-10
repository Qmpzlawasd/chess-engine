#include "Board.h"
#include "Move/Move.h"
#include "Utils.h"
#include <bitset>
int main() {

    const std::string asd = "2r5/8/7k/8/2R5/8/8/2K5 w - - 0 1";
    Board board{asd};
    uint64_t table = Bishop::getMoves(G5, 0);
    Utils::showBitBoard(table);
    std::vector <Move> as= board.getMovesFromBitboard(G5, table);
    std::for_each(as.begin(), as.end(), [](const Move &m) {
        std::cout << m;
    });

    //        MoveBuilder a{};
    //        std::cout << a.toSquare(A1).fromSquare(A2).withKingSideCastle();
    //        std::cout << board.rookAttacksSquare<WHITE>(C4) << '\n';
    //    std::cout << board.pawnAttacksSquare<WHITE>(D3) << '\n';
    //    Utils::showBitBoard(board.rooks.getBlockedAttackPattern(C8 , ~board.getEmptySquares()));
    //    Utils::showBitBoard(board.getPinnedSquares<BLACK>());
    //    Utils::showBitBoard(Pawn::getMoves<BLACK>(C7,board.getEmptySquares() & Utils::setSquare(C7)));
}
