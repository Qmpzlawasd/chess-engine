#include "Board/LegalMove.h"
#include "Game/Game.h"
#include "Perft/Perft.h"
#include "Uci.h"
#include <cstring>

int main(int argc, char *argv[]) {
    // idea when bishop is moved, restet only the danger tables for that sqare
    // draw by repetition
    // integrate hash
    // order by checks captures and attacks
    //    go wtime 50000 btime 50000 movestogo 40

    if (argc != 1) {
        if (strncmp(argv[1], "check", 5) == 0) {
            Perft::runTests();
            return 0;
        }
    }
    nnue_init("src/NNUE/nn-6b4236f2ec01.nnue");
    //    Board asd{"n1bqkbnr/pppppppp/8/8/8/N7/PPPPPPPP/RB1QKBNR w KQkq - 0 1"};
    //    for (int i = 0; i < 87; ++i) {
    //        Board aa{asd};
    //        if (asd.turn == WHITE) {
    //            LegalMove{asd}.getLegalMoves<WHITE>().value()[0]->makeMove(aa);
    //        } else {
    //            LegalMove{asd}.getLegalMoves<BLACK>().value()[0]->makeMove(aa);
    //        }
    //        std::cout << i << " : " << aa.callNNUE() << std::endl;
    //        asd=  aa;
    //    }
    Uci a;
    a.main();
}
