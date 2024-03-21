#include "LegalMove.h"

template <Color side>
std::vector<Move> LegalMove::getKingLegalMove() const noexcept {
    std::vector<Move> moves;

//    uint64_t kingBoard = board.king.getBitboard<side>();
//    const auto kingSquare = Utils::popLSB(kingBoard);

//    MoveBuilder moveBuilder;
//    const uint64_t basicMoves = ~board.getDangerTable<side>() & King::getMoves(board.king.getBitboard<side>());
//    constexpr Color color = side;
//    Utils::runForEachSetBit(
//        basicMoves,
//        [&moves, &moveBuilder, &kingSquare, this](const Square &square) noexcept -> void {
//            board.checkMoveIsCapture<side>(square);
//            moves.push_back(moveBuilder.fromSquare(kingSquare).toSquare(square).getMove());
//
//        });

    //    Utils::runForEachSetBit(board.getCastleRightsBitboard<side>(),
    //                            [&moves, &moveBuilder, &kingSquare](const Square &square) noexcept -> void {
    //                                if (square == C1 or square == C8) {
    //                                    moves.push_back(moveBuilder.fromSquare(kingSquare).toSquare(square).withQueenSideCastle());
    //                                } else {
    //                                    moves.push_back(moveBuilder.fromSquare(kingSquare).toSquare(square).withKingSideCastle());
    //                                }
    //                            });
    std::cout << moves.size();
    return moves;
}

template std::vector<Move> LegalMove::getKingLegalMove<WHITE>() const noexcept;
template std::vector<Move> LegalMove::getKingLegalMove<BLACK>() const noexcept;

template <Color side>
std::vector<Move> LegalMove::getRookLegalMove() const noexcept {
    return std::vector<Move>{};
}

template std::vector<Move> LegalMove::getRookLegalMove<WHITE>() const noexcept;
template std::vector<Move> LegalMove::getRookLegalMove<BLACK>() const noexcept;

template <Color side>
std::vector<Move> LegalMove::getPawnLegalMove() const noexcept {
    return std::vector<Move>{};
}

template std::vector<Move> LegalMove::getPawnLegalMove<WHITE>() const noexcept;
template std::vector<Move> LegalMove::getPawnLegalMove<BLACK>() const noexcept;

template <Color side>
std::vector<Move> LegalMove::getQueenLegalMove() const noexcept {
    return std::vector<Move>{};
}

template std::vector<Move> LegalMove::getQueenLegalMove<WHITE>() const noexcept;
template std::vector<Move> LegalMove::getQueenLegalMove<BLACK>() const noexcept;

template <Color side>
std::vector<Move> LegalMove::getKnightLegalMove() const noexcept {
    return std::vector<Move>{};
}

template std::vector<Move> LegalMove::getKnightLegalMove<WHITE>() const noexcept;
template std::vector<Move> LegalMove::getKnightLegalMove<BLACK>() const noexcept;

template <Color side>
std::vector<Move> LegalMove::getBishopLegalMove() const noexcept {
    return std::vector<Move>{};
}
template std::vector<Move> LegalMove::getBishopLegalMove<WHITE>() const noexcept;
template std::vector<Move> LegalMove::getBishopLegalMove<BLACK>() const noexcept;
