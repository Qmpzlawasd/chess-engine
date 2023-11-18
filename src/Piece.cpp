#pragma once

class Piece
{

    uint64_t white;
    uint64_t black;

public:
    Piece(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : white{whiteConfiguration}, black{blackConfiguration} {};
    const uint64_t getWhite() const { return white; }
    const uint64_t getBlack() const { return black; }
};

class Pawn : public Piece
{
public:
    Pawn(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
};

class Queen : public Piece
{
public:
    Queen(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
};

class Rook : public Piece
{
public:
    Rook(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
};
class Bishop : public Piece
{
public:
    Bishop(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
};
class King : public Piece
{
public:
    King(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
};

class Knight : public Piece
{
public:
    Knight(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
};
