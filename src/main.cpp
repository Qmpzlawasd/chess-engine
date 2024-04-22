#include "Board/LegalMove.h"
#include "Game/Game.h"
#include "Pertf.h"
#include <cstring>

int main(int argc, char *argv[]) {
    // idea when bishop is moved, restet only the danger tables for that sqare
    // draw by repetition
    // integrate hash
    // order by checks captures and attacks

    if (argc != 1) {
        if (strncmp(argv[1], "check", 5) == 0) {
            Pertf::runTests();
            return 0;
        }
    }
    Game::start("Capsunica", "Piersicuta");
}
