#include "Board.h"

[[nodiscard]] uint64_t Board::getFullWhiteSquares() const {
    return king.getWhite() | queens.getWhite() | rooks.getWhite() | bishops.getWhite() | knights.getWhite() | pawns.getWhite();
}

[[nodiscard]] uint64_t Board::getFullBlackSquares() const {
    return king.getBlack() | queens.getBlack() | rooks.getBlack() | bishops.getBlack() | knights.getBlack() | pawns.getBlack();
}

[[nodiscard]] uint64_t Board::getEmptySquares() const { return ~(getFullBlackSquares() | getFullWhiteSquares()); }

void Board::printBoard(std::ostream &os) const {}

void Board::printStatus(std::ostream &os) const {
    os << "Move " << fullmoveNumber << ": " << (castle[1] ? "Q" : "") << (castle[3] ? "q" : "") << "\t" << (int)halfmoveClock << "\t"
       << enPassant << "\t" << (castle[0] ? "K" : "") << (castle[2] ? "k" : "") << "\n";
}

template <bool side> [[nodiscard]] uint64_t Board::getKightMoves() const {
    uint64_t pieceBitboard;
    if constexpr (side == Utils::WHITE)
        pieceBitboard = knights.getWhite();
    else
        pieceBitboard = knights.getBlack();

    constexpr uint64_t allyPieces;
    if constexpr (side == Utils::WHITE)
        allyPieces = this->getFullWhiteSquares();
    else
        allyPieces = this->getFullBlackSquares();

    constexpr uint64_t B_FILE = Utils::A_FILE << 1;
    constexpr uint64_t G_FILE = Utils::H_FILE >> 1;

    constexpr uint64_t SECOND_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * 1;
    constexpr uint64_t EIGHTH_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * (Utils::ROW_NUMBER - 1);
    constexpr uint64_t SEVENTH_ROW = EIGHTH_ROW >> 1;

    const uint64_t attackLeftUpUp = (pieceBitboard & ~(SEVENTH_ROW | EIGHTH_ROW | Utils::A_FILE)) << (Utils::ROW_NUMBER * 2 - 1);
    const uint64_t attackLeftUpLeft = (pieceBitboard & ~(Utils::A_FILE | B_FILE | EIGHTH_ROW)) << (Utils::ROW_NUMBER - 2);
    const uint64_t attackLeftDownDown = (pieceBitboard & ~(Utils::FIRST_ROW | SECOND_ROW | Utils::A_FILE)) >> (Utils::ROW_NUMBER * 2 + 1);
    const uint64_t attackLeftDownLeft = (pieceBitboard & ~(Utils::A_FILE | Utils::FIRST_ROW | B_FILE)) >> (Utils::ROW_NUMBER + 2);

    const uint64_t attackRightUpUp = (pieceBitboard & ~(SEVENTH_ROW | EIGHTH_ROW | Utils::H_FILE)) << (Utils::ROW_NUMBER * 2 + 1);
    const uint64_t attackRightUpRight = (pieceBitboard & ~(Utils::H_FILE | G_FILE | EIGHTH_ROW)) << (Utils::ROW_NUMBER + 2);

    const uint64_t attackRightDownDown = (pieceBitboard & ~(Utils::FIRST_ROW | SECOND_ROW | Utils::H_FILE)) >> (Utils::ROW_NUMBER * 2 - 1);
    const uint64_t attackRightDownRight = (pieceBitboard & ~(Utils::H_FILE | Utils::FIRST_ROW | G_FILE)) >> (Utils::ROW_NUMBER - 2);

    return (attackLeftUpUp | attackLeftUpLeft | attackLeftDownDown | attackLeftDownLeft | attackRightUpUp | attackRightUpRight |
            attackRightDownDown | attackRightDownRight) &
           ~allyPieces;
}

[[nodiscard]] uint64_t Board::getRookMoves(const Square &square) const {
    const uint64_t blockerPattern = Rook::getNaiveAttackPattern(square) & ~getEmptySquares();

    std::shared_ptr<MagicValuesGeneratorInterface> valueGenerator =
        std::make_shared<MagicValuesParallelGenerator>(MagicValuesParallelGenerator(ROOK_SHIFT, blockerPattern));
    const MagicBitboard magicBoard{valueGenerator};

    std::vector<std::vector<uint64_t>> rookMoveTable;
    rookMoveTable = magicBoard.magicGenerator->getTables(ROOK_CONSTANTS, rooks);

    return rookMoveTable[square][blockerPattern * ROOK_CONSTANTS[square] >> ROOK_SHIFT];
}

[[nodiscard]] uint64_t Board::getBishopMoves(const Square &square) const {
    const uint64_t blockerPattern = Bishop::getNaiveAttackPattern(square) & ~getEmptySquares();

    std::shared_ptr<MagicValuesGeneratorInterface> base =
        std::make_shared<MagicValuesParallelGenerator>(MagicValuesParallelGenerator{BISHOP_SHIFT, blockerPattern});
    const MagicBitboard magicBoard{base};

    std::vector<std::vector<uint64_t>> bishopMoveTable;
    bishopMoveTable = magicBoard.magicGenerator->getTables(BISHOP_CONSTANTS, bishops);

    return bishopMoveTable[square][blockerPattern * BISHOP_CONSTANTS[square] >> BISHOP_SHIFT];
}

template <bool side> [[nodiscard]] uint64_t Board::getPawnMoves() const {
    uint64_t pieceBitboard;
    if constexpr (side == Utils::WHITE)
        pieceBitboard = pawns.getWhite();
    else
        pieceBitboard = pawns.getBlack();

    uint64_t forwardMove, attackLeft, attackRight;
    if constexpr (side == Utils::WHITE) {
        forwardMove = pieceBitboard << Utils::ROW_NUMBER;
        attackLeft = ((pieceBitboard & ~Utils::A_FILE) << (Utils::ROW_NUMBER - 1)) & (getFullBlackSquares() | enPassant);
        attackRight = ((pieceBitboard & ~Utils::H_FILE) << (Utils::ROW_NUMBER + 1)) & (getFullBlackSquares() | enPassant);
    } else {
        // move orientations are reversed
        forwardMove = pieceBitboard >> Utils::ROW_NUMBER;
        attackLeft = ((pieceBitboard & ~Utils::H_FILE) >> (Utils::ROW_NUMBER - 1)) & (getFullWhiteSquares() | enPassant);
        attackRight = ((pieceBitboard & ~Utils::A_FILE) >> (Utils::ROW_NUMBER + 1)) & (getFullWhiteSquares() | enPassant);
    }
    forwardMove = forwardMove & getEmptySquares();

    return forwardMove | attackLeft | attackRight;
}

template <bool side> [[nodiscard]] uint64_t Board::getKingMoves() const {
    uint64_t pieceBitboard;
    if constexpr (side == Utils::WHITE)
        pieceBitboard = king.getWhite();
    else
        pieceBitboard = king.getBlack();

    constexpr uint64_t allyPieces;
    if constexpr (side == Utils::WHITE)
        allyPieces = this->getFullWhiteSquares();
    else
        allyPieces = this->getFullBlackSquares();

    const uint64_t attackLeft = (pieceBitboard & ~Utils::A_FILE) >> 1;
    const uint64_t attackRight = (pieceBitboard & ~Utils::H_FILE) << 1;

    const uint64_t attackUp = (pieceBitboard >> Utils::ROW_NUMBER);
    const uint64_t attackDown = (pieceBitboard << Utils::ROW_NUMBER);

    const uint64_t attackLeftUp = (pieceBitboard & ~Utils::A_FILE) << (Utils::ROW_NUMBER - 1);
    const uint64_t attackLeftDown = (pieceBitboard & ~Utils::A_FILE) >> (Utils::ROW_NUMBER + 1);
    const uint64_t attackRightUp = (pieceBitboard & ~Utils::H_FILE) << (Utils::ROW_NUMBER + 1);
    const uint64_t attackRightDown = (pieceBitboard & ~Utils::H_FILE) >> (Utils::ROW_NUMBER - 1);

    return (attackLeftUp | attackLeftDown | attackRightUp | attackRightDown | attackLeft | attackRight | attackUp | attackDown) &
           ~allyPieces;
}
