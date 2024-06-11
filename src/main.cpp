#include "Board/LegalMove.h"
#include "Game/Game.h"
#include "Perft/Perft.h"
#include "Uci.h"
#include <cstring>

int main(int argc, char *argv[]) {
    if (argc != 1) {
        if (strncmp(argv[1], "test", 5) == 0) {
            Perft::runTests();
            return 0;
        }
    }

    Uci{}.main();
}
