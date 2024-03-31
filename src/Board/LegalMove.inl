#ifndef CHESS_ENGINE_LEGALMOVE_INL
#define CHESS_ENGINE_LEGALMOVE_INL

#include "../Move/Move.h"
#include "../Move/MoveBuilder.h"
#include "LegalMove.h"

template <Color side>
std::vector<Move> LegalMove::getKingLegalMoves() const noexcept {
    std::vector<Move> moves;

    uint64_t kingBoard = board.king.getBitboard<side>();
    const auto kingSquare = Utils::popLSB(kingBoard);

    MoveBuilder moveBuilder;
    const uint64_t basicMoves = ~board.computeDangerTable<side>() & King::getMoves(board.king.getBitboard<side>());

    Utils::runForEachSetBit(basicMoves | board.getCastleRightsBitboard<side>(),
                            [&moves, &moveBuilder, &kingSquare, this](const Square &square) noexcept -> void {
                                moves.push_back(moveBuilder.fromSquare(kingSquare).toSquare(square).getMove());
                            });

    return moves;
}

template <Color side>
std::vector<Move> LegalMove::getRookLegalMoves() const noexcept {
    return std::vector<Move>{};
}

template <Color side>
std::vector<Move> LegalMove::getPawnLegalMoves() const noexcept {

    std::vector<Move> moves;
    MoveBuilder moveBuilder;
    uint64_t moveTable = 0;
    const uint64_t pawnsCannotAttack = board.pawns.getBitboard<side>() & board.getPinnedMaskHV<side>();
    const uint64_t pawnsCannotBePushed = board.pawns.getBitboard<side>() & board.getPinnedMaskD12<side>();
    const uint64_t freePawns = board.pawns.getBitboard<side>() ^ (pawnsCannotBePushed | pawnsCannotAttack);

    Utils::runForEachSetBit(pawnsCannotAttack, [this, &moveBuilder, &moves, &moveTable](const Square &originSquare) {
        const uint64_t legalMoves = Pawn::getMoves<side>(originSquare, board.getEmptySquares()) & board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves, &moveTable](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).getMove());
            moveTable |= Utils::setSquare(square);
        });
    });

    Utils::runForEachSetBit(pawnsCannotBePushed, [this, &moveBuilder, &moves, &moveTable](const Square &originSquare) {
        const uint64_t legalMovesWithEnpassant = Pawn::getThreatens<side>(originSquare) &
                                                 (board.getOccupiedSquares<Utils::flipColor(side)>() | board.enPassant) &
                                                 board.getCheckMask<side>() & board.getPinnedMaskD12<side>();
        Utils::runForEachSetBit(legalMovesWithEnpassant, [&originSquare, &moveBuilder, &moves, &moveTable](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).getMove());
            moveTable |= Utils::setSquare(square);
        });
    });

    Utils::runForEachSetBit(freePawns, [this, &moveBuilder, &moves, &moveTable](const Square &originSquare) {
        const uint64_t legalMovesWithEnpassant = Pawn::getThreatens<side>(originSquare) &
                                                 (board.getOccupiedSquares<Utils::flipColor(side)>() | board.enPassant) &
                                                 board.getCheckMask<side>();

        const uint64_t legalPushes = Pawn::getMoves<side>(originSquare, board.getEmptySquares()) & board.getCheckMask<side>();

        Utils::runForEachSetBit(legalPushes | legalMovesWithEnpassant,
                                [&originSquare, &moveBuilder, &moves, &moveTable](const Square &square) {
                                    moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).getMove());
                                    moveTable |= Utils::setSquare(square);
                                });
    });
    Utils::showBitBoard(moveTable);
    return moves;
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
