#pragma once
#include "Utils.cpp"
#include "Piece.cpp"

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
    std::string enPassant;
    uint8_t halfmoveClock;
    uint16_t fullmoveNumber;

    const __uint128_t getFullBoard() const
    {
        return ((__uint128_t)(king.getBlack()) |
                (queens.getBlack()) |
                (knights.getBlack()) |
                (bishops.getBlack()) |
                (pawns.getBlack()) |
                (rooks.getBlack()))
                   << (Utils::COLUMN_NUMBER * Utils::ROW_NUMBER) |
               (king.getWhite()) |
               (queens.getWhite()) |
               (knights.getWhite()) |
               (bishops.getWhite()) |
               (pawns.getWhite()) |
               (rooks.getWhite());
    }

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
        return ~(king.getBlack()) |
               (queens.getBlack()) |
               (knights.getBlack()) |
               (bishops.getBlack()) |
               (pawns.getBlack()) |
               (rooks.getBlack()) |
               (king.getWhite()) |
               (queens.getWhite()) |
               (knights.getWhite()) |
               (bishops.getWhite()) |
               (pawns.getWhite()) |
               (rooks.getWhite());
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

    const uint64_t getKightMoves(const bool &side) const
    {
        const uint64_t pieceBitboard = side ? knights.getWhite() : knights.getBlack();
        const uint64_t allyPieces = side ? this->getFullWhiteSquares() : this->getFullBlackSquares();

        constexpr uint64_t B_FILE = Utils::A_FILE << 1;
        constexpr uint64_t G_FILE = Utils::H_FILE >> 1;

        constexpr uint64_t SECOND_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * 1;
        constexpr uint64_t EIGHTH_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * 7;

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

    const uint64_t getKingMoves(const bool &side) const
    {
        const uint64_t pieceBitboard = side ? king.getWhite() : king.getBlack();
        const uint64_t allyPieces = side ? this->getFullWhiteSquares() : this->getFullBlackSquares();

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
