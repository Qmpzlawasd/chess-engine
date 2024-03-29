#ifndef CHESS_ENGINE_LEGALMOVE_INL
#define CHESS_ENGINE_LEGALMOVE_INL

#include "../Move/Move.h"
#include "../Move/MoveBuilder.h"
#include "LegalMove.h"

template <Color side>
std::vector<Move> LegalMove::getKingLegalMoves() const noexcept {
    std::vector<Move> moves;

//    uint64_t kingBoard = board.king.getBitboard<side>();
//    const auto kingSquare = Utils::popLSB(kingBoard);
//
//    MoveBuilder moveBuilder;
//    const uint64_t basicMoves = ~board.computeDangerTable<side>() & King::getMoves(board.king.getBitboard<side>());
//
//    Utils::runForEachSetBit(basicMoves | board.getCastleRightsBitboard<side>(),
//                            [&moves, &moveBuilder, &kingSquare, this](const Square &square) noexcept -> void {
//                                moves.push_back(moveBuilder.fromSquare(kingSquare).toSquare(square).getMove());
//                            });

    return moves;
}

template <Color side>
std::vector<Move> LegalMove::getRookLegalMoves() const noexcept {
    return std::vector<Move>{};
}

template <Color side>
std::vector<Move> LegalMove::getPawnLegalMoves() const noexcept {
    return std::vector<Move>{};
}

template <Color side>
std::vector<Move> LegalMove::getQueenLegalMoves() const noexcept {
    return std::vector<Move>{};
}

template <Color side>
std::vector<Move> LegalMove::getKnightLegalMoves() const noexcept {
    std::vector<Move> moves;
    const uint64_t freeKnights = board.knights.getBitboard<side>() ^ ( board.knights.getBitboard<side>() & (board.getPinnedMaskD12<side>() | board.getPinnedMaskHV<side>()));
    Utils::showBitBoard(freeKnights);
    MoveBuilder moveBuilder = MoveBuilder{};

    Utils::runForEachSetBit(freeKnights, [&moveBuilder, this, &moves](const Square &originSquare) -> void {
        Utils::runForEachSetBit(Knight::getMoves(originSquare) & ~board.getOccupiedSquares<side>() & board.getCheckMask<side>(),
                                [&moveBuilder, &originSquare, &moves](const Square &legalSquare) -> void {
                                    moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(legalSquare).withKnight().getMove());
                                });
    });
    return moves;
}

template <Color side>
std::vector<Move> LegalMove::getBishopLegalMoves() const noexcept {
    return std::vector<Move>{};
}

#endif // CHESS_ENGINE_LEGALMOVE_INL