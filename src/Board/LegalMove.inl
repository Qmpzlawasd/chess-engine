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

    std::vector<Move> moves;
    MoveBuilder moveBuilder;
    const uint64_t pinnedRooksCannotMove = board.rooks.getBitboard<side>() & board.getPinnedMaskD12<side>();

    const uint64_t pinnedRooks = board.rooks.getBitboard<side>() & board.getPinnedMaskHV<side>();
    const uint64_t freeRooks = board.rooks.getBitboard<side>() ^ (pinnedRooksCannotMove | pinnedRooks);

    Utils::runForEachSetBit(pinnedRooks, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Rook::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getPinnedMaskHV<side>() & board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withRook().getMove());
        });
    });
    Utils::runForEachSetBit(freeRooks, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Rook::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withRook().getMove());
        });
    });
    return moves;
}

template <Color side>
std::vector<Move> LegalMove::getPawnLegalMoves() const noexcept {

    std::vector<Move> moves;
    MoveBuilder moveBuilder;

    const uint64_t pawnsCannotAttack = board.pawns.getBitboard<side>() & board.getPinnedMaskHV<side>();
    const uint64_t pawnsCannotBePushed = board.pawns.getBitboard<side>() & board.getPinnedMaskD12<side>();
    const uint64_t freePawns = board.pawns.getBitboard<side>() ^ (pawnsCannotBePushed | pawnsCannotAttack);

    Utils::runForEachSetBit(pawnsCannotAttack, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Pawn::getMoves<side>(originSquare, board.getEmptySquares()) & board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).getMove());
        });
    });

    Utils::runForEachSetBit(pawnsCannotBePushed, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMovesWithEnpassant = Pawn::getThreatens<side>(originSquare) &
                                                 (board.getOccupiedSquares<Utils::flipColor(side)>() | board.enPassant) &
                                                 board.getCheckMask<side>() & board.getPinnedMaskD12<side>();
        Utils::runForEachSetBit(legalMovesWithEnpassant, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).getMove());
        });
    });

    Utils::runForEachSetBit(freePawns, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t canDoEnPassant = handlePawnShenanigans<side>() ? board.enPassant : 0;

        const uint64_t legalMovesWithEnpassant = Pawn::getThreatens<side>(originSquare) &
                                                 (board.getOccupiedSquares<Utils::flipColor(side)>() | canDoEnPassant) &
                                                 board.getCheckMask<side>();

        const uint64_t legalPushes = Pawn::getMoves<side>(originSquare, board.getEmptySquares()) & board.getCheckMask<side>();

        Utils::runForEachSetBit(legalPushes | legalMovesWithEnpassant, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).getMove());
        });
    });
    return moves;
}

template <Color side>
[[nodiscard]] bool LegalMove::handlePawnShenanigans() noexcept {
    uint64_t controversialPawn;
    if constexpr (side == WHITE) {
        controversialPawn = board.enPassant >> Utils::COLUMN_NUMBER;
        board.pawns.flipSquare<BLACK>(Utils::popLSB(controversialPawn));
    } else {
        controversialPawn = board.enPassant << Utils::COLUMN_NUMBER;
        board.pawns.flipSquare<WHITE>(Utils::popLSB(controversialPawn));
    }

    uint64_t king = board.king.getBitboard<side>();
    const uint64_t isPinned = board.queenAttacksSquare<side>(Utils::popLSB(king));

    // undo
    if constexpr (side == WHITE) {
        board.pawns.flipSquare<BLACK>(Utils::popLSB(controversialPawn));
    } else {
        board.pawns.flipSquare<WHITE>(Utils::popLSB(controversialPawn));
    }
    return isPinned;
}

template <Color side>
std::vector<Move> LegalMove::getQueenLegalMoves() const noexcept {
    std::vector<Move> moves;
    MoveBuilder moveBuilder;

    const uint64_t pinnedQueensHV = board.queens.getBitboard<side>() & board.getPinnedMaskHV<side>();
    const uint64_t pinnedQueensD12 = board.queens.getBitboard<side>() & board.getPinnedMaskD12<side>();
    const uint64_t freeQueens = board.queens.getBitboard<side>() ^ (pinnedQueensD12 | pinnedQueensHV);

    Utils::runForEachSetBit(pinnedQueensD12, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Queen::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getPinnedMaskD12<side>() & board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withBishop().getMove());
        });
    });

    Utils::runForEachSetBit(pinnedQueensHV, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Queen::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getPinnedMaskHV<side>() & board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withBishop().getMove());
        });
    });

    Utils::runForEachSetBit(freeQueens, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Queen::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withBishop().getMove());
        });
    });

    return moves;
}

template <Color side>
std::vector<Move> LegalMove::getKnightLegalMoves() const noexcept {
    std::vector<Move> moves;
    const uint64_t freeKnights = board.knights.getBitboard<side>() ^
                                 (board.knights.getBitboard<side>() & (board.getPinnedMaskD12<side>() | board.getPinnedMaskHV<side>()));
    uint64_t ads = 0;
    MoveBuilder moveBuilder = MoveBuilder{};

    Utils::runForEachSetBit(freeKnights, [&moveBuilder, this, &moves, &ads](const Square &originSquare) -> void {
        Utils::runForEachSetBit(Knight::getMoves(originSquare) & ~board.getOccupiedSquares<side>() & board.getCheckMask<side>(),
                                [&moveBuilder, &originSquare, &moves, &ads](const Square &legalSquare) -> void {
                                    moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(legalSquare).withKnight().getMove());
                                    ads |= Utils::setSquare(legalSquare);
                                });
    });
    Utils::showBitBoard(ads);
    return moves;
}

template <Color side>
std::vector<Move> LegalMove::getBishopLegalMoves() const noexcept {

    std::vector<Move> moves;
    MoveBuilder moveBuilder;
    const uint64_t pinnedBishopsCannotMove = board.bishops.getBitboard<side>() & board.getPinnedMaskHV<side>();

    const uint64_t pinnedBishops = board.bishops.getBitboard<side>() & board.getPinnedMaskD12<side>();
    const uint64_t freeBishops = board.bishops.getBitboard<side>() ^ (pinnedBishopsCannotMove | pinnedBishops);

    Utils::runForEachSetBit(pinnedBishops, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Bishop::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getPinnedMaskD12<side>() & board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withBishop().getMove());
        });
    });

    Utils::runForEachSetBit(freeBishops, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Bishop::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withBishop().getMove());
        });
    });

    return moves;
}

#endif // CHESS_ENGINE_LEGALMOVE_INL
