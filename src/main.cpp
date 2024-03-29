#include "Board/LegalMove.h"

int main() {

    const std::string fenString = "1k5n/2N5/b7/4N1r1/8/8/4K3/8 w - - 0 1";
    Board board{fenString};
    LegalMove legalMove{board};

    //    Utils::showBitBoard(board.computeCheckMask<BLACK>());
    std::vector<Move> moves = legalMove.getKnightLegalMoves<WHITE>();
    std::for_each(moves.begin(), moves.end(), [](const Move &move) -> void { std::cout << move; });

    // const uint64_t a = board.getPinMaskHV<WHITE>();
    // const uint64_t b = board.getPinMaskD12<WHITE>();
    // Utils::showBitBoard(a);
    // Utils::showBitBoard(b);

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
