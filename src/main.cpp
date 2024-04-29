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

    if (argc != 1) {
        if (strncmp(argv[1], "check", 5) == 0) {
            Perft::runTests();
            return 0;
        }
    }
        Uci a;
        a.main();
}
