#pragma once
#include "Utils.cpp"
#include <unordered_map>
#include "Piece.cpp"

enum Square
{
    A1,
    B1,
    C1,
    D1,
    E1,
    F1,
    G1,
    H1,
    A2,
    B2,
    C2,
    D2,
    E2,
    F2,
    G2,
    H2,
    A3,
    B3,
    C3,
    D3,
    E3,
    F3,
    G3,
    H3,
    A4,
    B4,
    C4,
    D4,
    E4,
    F4,
    G4,
    H4,
    A5,
    B5,
    C5,
    D5,
    E5,
    F5,
    G5,
    H5,
    A6,
    B6,
    C6,
    D6,
    E6,
    F6,
    G6,
    H6,
    A7,
    B7,
    C7,
    D7,
    E7,
    F7,
    G7,
    H7,
    A8,
    B8,
    C8,
    D8,
    E8,
    F8,
    G8,
    H8,
};
class Board
{
public:
    King king;
    Queen queens;
    Rook rooks;
    Bishop bishops;
    Knight knights;
    Pawn pawns;
    /**
     * true -> it's white's turn
     */
    bool turn;
    /**
     * last bit -> can white castle  castle kingside
     * second bit -> can white castle queenside
     * etc.
     */
    std::bitset<4> castle;
    uint64_t enPassant;
    uint8_t halfmoveClock;
    uint16_t fullmoveNumber;

    const uint64_t getFullWhiteSquares() const
    {
        return king.getWhite() |
               queens.getWhite() |
               rooks.getWhite() |
               bishops.getWhite() |
               knights.getWhite() |
               pawns.getWhite();
    }

    const uint64_t getFullBlackSquares() const
    {
        return king.getBlack() |
               queens.getBlack() |
               rooks.getBlack() |
               bishops.getBlack() |
               knights.getBlack() |
               pawns.getBlack();
    }

    const uint64_t getEmptySquares() const
    {
        return ~(getFullBlackSquares() | getFullWhiteSquares());
    }

    void printBoard(std::ostream &os) const
    {
    }

    void printStatus(std::ostream &os) const
    {

        os << "Move " << fullmoveNumber << ": "
           << (castle[1] ? "Q" : "")
           << (castle[3] ? "q" : "") << "\t"
           << (int)halfmoveClock << "\t"
           << enPassant << "\t"
           << (castle[0] ? "K" : "")
           << (castle[2] ? "k" : "")
           << "\n";
    }

public:
    Board(const std::string &fen = "1nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
        : king{FEN::parsePiece(fen, 'K'), FEN::parsePiece(fen, 'k')},
          queens{FEN::parsePiece(fen, 'Q'), FEN::parsePiece(fen, 'q')},
          rooks{FEN::parsePiece(fen, 'R'), FEN::parsePiece(fen, 'r')},
          bishops{FEN::parsePiece(fen, 'B'), FEN::parsePiece(fen, 'b')},
          knights{FEN::parsePiece(fen, 'N'), FEN::parsePiece(fen, 'n')},
          pawns{FEN::parsePiece(fen, 'P'), FEN::parsePiece(fen, 'p')},
          turn{FEN::parseTurn(fen)},
          castle{FEN::parseCastle(fen)},
          enPassant{FEN::parseEnPassant(fen)},
          halfmoveClock{FEN::parseHalfmoveClock(fen)},
          fullmoveNumber{FEN::parseFullmoveNumber(fen)} {};

    ~Board() = default;

    template <bool side>
    const uint64_t getKightMoves() const
    {
        const uint64_t pieceBitboard = side == Utils::WHITE ? knights.getWhite() : knights.getBlack();
        const uint64_t allyPieces = side == Utils::WHITE ? this->getFullWhiteSquares() : this->getFullBlackSquares();

        constexpr uint64_t B_FILE = Utils::A_FILE << 1;
        constexpr uint64_t G_FILE = Utils::H_FILE >> 1;

        constexpr uint64_t SECOND_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * 1;
        constexpr uint64_t EIGHTH_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * (Utils::ROW_NUMBER - 1);
        constexpr uint64_t SEVENTH_ROW = EIGHTH_ROW >> 1;

        const uint64_t attackLeftUpUp = (pieceBitboard & ~(SEVENTH_ROW | EIGHTH_ROW | Utils::A_FILE)) << Utils::ROW_NUMBER * 2 - 1;
        const uint64_t attackLeftUpLeft = (pieceBitboard & ~(Utils::A_FILE | B_FILE | EIGHTH_ROW)) << Utils::ROW_NUMBER - 2;
        const uint64_t attackLeftDownDown = (pieceBitboard & ~(Utils::FIRST_ROW | SECOND_ROW | Utils::A_FILE)) >> Utils::ROW_NUMBER * 2 + 1;
        const uint64_t attackLeftDownLeft = (pieceBitboard & ~(Utils::A_FILE | Utils::FIRST_ROW | B_FILE)) >> Utils::ROW_NUMBER + 2;

        const uint64_t attackRightUpUp = (pieceBitboard & ~(SEVENTH_ROW | EIGHTH_ROW | Utils::H_FILE)) << Utils::ROW_NUMBER * 2 + 1;
        const uint64_t attackRightUpRight = (pieceBitboard & ~(Utils::H_FILE | G_FILE | EIGHTH_ROW)) << Utils::ROW_NUMBER + 2;

        const uint64_t attackRightDownDown = (pieceBitboard & ~(Utils::FIRST_ROW | SECOND_ROW | Utils::H_FILE)) >> Utils::ROW_NUMBER * 2 - 1;
        const uint64_t attackRightDownRight = (pieceBitboard & ~(Utils::H_FILE | Utils::FIRST_ROW | G_FILE)) >> Utils::ROW_NUMBER - 2;

        return (attackLeftUpUp | attackLeftUpLeft | attackLeftDownDown | attackLeftDownLeft | attackRightUpUp | attackRightUpRight | attackRightDownDown | attackRightDownRight) & ~allyPieces;
    }

    // const uint64_t generateRookMoves() const{}
    const uint64_t getRookMoves(const Square &square) const
    {
        const uint8_t line = square % Utils::ROW_NUMBER;
        const uint8_t column = square / Utils::ROW_NUMBER;

        const uint64_t PIECE_FILE = Utils::A_FILE << line;
        const uint64_t PIECE_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * column;

        const uint64_t attackPattern = PIECE_FILE | PIECE_ROW;
        std::unordered_map<uint32_t, uint64_t> magic{MagicBitboard::generateRookPattern<uint32_t>(attackPattern)};

        return magic[attackPattern];
    }

    template <bool side>
    const uint64_t getPawnMoves() const
    {
        uint64_t pieceBitboard;
        if constexpr (side == Utils::WHITE)
            pieceBitboard = rooks.getWhite();
        else
            pieceBitboard = rooks.getBlack();

        uint64_t forwardMove, attackLeft, attackRight;
        if constexpr (side == Utils::WHITE)
        {
            forwardMove = pieceBitboard << Utils::ROW_NUMBER;
            attackLeft = ((pieceBitboard & ~Utils::A_FILE) << Utils::ROW_NUMBER - 1) & (getFullBlackSquares() | enPassant);
            attackRight = ((pieceBitboard & ~Utils::H_FILE) << Utils::ROW_NUMBER + 1) & (getFullBlackSquares() | enPassant);
        }
        else
        {
            // move orientations are reversed
            forwardMove = pieceBitboard >> Utils::ROW_NUMBER;
            attackLeft = ((pieceBitboard & ~Utils::H_FILE) >> Utils::ROW_NUMBER - 1) & (getFullWhiteSquares() | enPassant);
            attackRight = ((pieceBitboard & ~Utils::A_FILE) >> Utils::ROW_NUMBER + 1) & (getFullWhiteSquares() | enPassant);
        }
        forwardMove = forwardMove & getEmptySquares();

        return forwardMove | attackLeft | attackRight;
    }

    template <bool side>
    const uint64_t getKingMoves() const
    {
        uint64_t pieceBitboard;
        if constexpr (side == Utils::WHITE)
            pieceBitboard = rooks.getWhite();
        else
            pieceBitboard = rooks.getBlack();

        const uint64_t allyPieces;
        if constexpr (side == Utils::WHITE)
            this->getFullWhiteSquares();
        else
            this->getFullBlackSquares();

        const uint64_t attackLeft = (pieceBitboard & ~Utils::A_FILE) >> 1;
        const uint64_t attackRight = (pieceBitboard & ~Utils::H_FILE) << 1;

        const uint64_t attackUp = (pieceBitboard >> Utils::ROW_NUMBER);
        const uint64_t attackDown = (pieceBitboard << Utils::ROW_NUMBER);

        const uint64_t attackLeftUp = (pieceBitboard & ~Utils::A_FILE) << Utils::ROW_NUMBER - 1;
        const uint64_t attackLeftDown = (pieceBitboard & ~Utils::A_FILE) >> Utils::ROW_NUMBER + 1;
        const uint64_t attackRightUp = (pieceBitboard & ~Utils::H_FILE) << Utils::ROW_NUMBER + 1;
        const uint64_t attackRightDown = (pieceBitboard & ~Utils::H_FILE) >> Utils::ROW_NUMBER - 1;

        return (attackLeftUp | attackLeftDown | attackRightUp | attackRightDown | attackLeft | attackRight | attackUp | attackDown) & ~allyPieces;
    };

    friend std::ostream &operator<<(std::ostream &os, const Board &board)
    {
        board.printStatus(os);
        board.printBoard(os);
        return os;
    }
};
