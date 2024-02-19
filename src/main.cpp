#include "Board.h"
#include "Move.h"
#include "Utils.h"
#include <bitset>
int main() {

    const std::string asd = "8/8/8/3p4/8/4P3/8/8 w - - 0 1";
    Board board{asd};
    //    Utils::showBitBoard(board.getBishopMoves<WHITE>(G5));
//        MoveBuilder a{};
//        std::cout << a.toSquare(A1).fromSquare(A2).withKingSideCastle();
//    std::cout << board.rookAttacksSquare<WHITE>(C4) << '\n';
    std::cout << board.pawnAttacksSquare<BLACK>(C4) << '\n';
    std::cout << board.pawnAttacksSquare<BLACK>(D4) << '\n';
    std::cout << board.pawnAttacksSquare<BLACK>(E4) << '\n';
}
