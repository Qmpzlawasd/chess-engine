//#include "../src/Board.h"
#include <gtest/gtest.h>
// #include "../src/FEN.h"
// #include "../src/MagicBitboard.h"
// #include "../src/MagicValuesGeneratorInterface.h"
// #include "../src/MagicValuesParallelGenerator.h"
// #include "../src/Piece.h"
// #include "../src/Squares.h"
// #include "../src/Utils.h"
#include <vector>
namespace {

TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}
//TEST(HelloTest, ASD) {
//    const std::string asd = "2NB1B2/B3r3/qr4N1/1rb5/6rn/1r2N3/2r3pk/3rN1RP w - - 0 1";
//    Board board{asd};
//    EXPECT_EQ(board.getRookMoves<Utils::WHITE>(G5), 808527593472UL);
//}
} // namespace