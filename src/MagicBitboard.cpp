#pragma once
#include "Utils.cpp"
#include "Piece.cpp"
#include "MagicBoardGenerator.cpp"
#include <memory>

class MagicBitboard
{
public:
     std::shared_ptr<MagicBoardGenerator> magicGenerator;
    MagicBitboard(std::shared_ptr<MagicBoardGenerator> & _magicGenerator) : magicGenerator{_magicGenerator} {};

};