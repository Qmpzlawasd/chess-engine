#pragma once

class Piece
{

    std::bitset<Utils::ROW_NUMBER * Utils::COLUMN_NUMBER> white;
    std::bitset<Utils::ROW_NUMBER * Utils::COLUMN_NUMBER> black;

public:
    Piece(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : white{whiteConfiguration}, black{blackConfiguration} {};
    const uint64_t getWhite() const { return white.to_ulong(); }
    const uint64_t getBlack() const { return black.to_ulong(); }
    /**
     * Calculates pseudo-legal moves
     *
     * @param value true if it's white's turn, else false
     * @returns bitboard
     */
    virtual const uint64_t getPseudoLegalMoves(const bool &) const = 0;
    virtual const uint64_t movePiece(const uint64_t &, const uint8_t &) const = 0;
};

class Pawn : public Piece
{
    const uint64_t movePiece(const uint64_t &board, const uint8_t &index) const override
    {
        uint64_t newBoard{0};
        return newBoard;
    };

public:
    Pawn(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
    const uint64_t getPseudoLegalMoves(const bool &turn) const override
    {
        uint64_t attackTable{0};
        if (turn == Utils::WHITE)
        {
            Utils::showBitBoard(this->getWhite());
            for (uint8_t i = 0; i < Utils::COLUMN_NUMBER * Utils::ROW_NUMBER; i++)
            {
                if (this->getWhite() >> i & 1)
                {
                    std::cout << (this->getWhite() >> i & 1) << '\n';
                    attackTable |= this->movePiece(this->getWhite(), i); // e mai complicat
                }
            }
        }
        else
        {
            Utils::showBitBoard(this->getBlack());
        }
        return 1;
    }
};

class Queen : public Piece
{
    const uint64_t movePiece(const uint64_t &board, const uint8_t &index) const override { return 1; };

public:
    Queen(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
    const uint64_t getPseudoLegalMoves(const bool &turn) const override { return 1; };
};

class Rook : public Piece
{
    const uint64_t movePiece(const uint64_t &board, const uint8_t &index) const override { return 1; };

public:
    Rook(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
    const uint64_t getPseudoLegalMoves(const bool &turn) const override { return 1; };
};
class Bishop : public Piece
{
    const uint64_t movePiece(const uint64_t &board, const uint8_t &index) const override { return 1; };

public:
    Bishop(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
    const uint64_t getPseudoLegalMoves(const bool &turn) const override { return 1; };
};
class King : public Piece
{
    const uint64_t movePiece(const uint64_t &board, const uint8_t &index) const override { return 1; };

public:
    King(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
    const uint64_t getPseudoLegalMoves(const bool &turn) const override { return 1; };
};

class Knight : public Piece
{

    const uint64_t movePiece(const uint64_t &board, const uint8_t &index) const override { return 1; };

public:
    Knight(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
    const uint64_t getPseudoLegalMoves(const bool &turn) const override { return 1; };
};
