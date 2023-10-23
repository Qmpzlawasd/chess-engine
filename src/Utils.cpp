#pragma once
#include <math.h>

class Utils
{
public:
    static const bool WHITE = true;
    static const bool BLACK = false;
    static const uint8_t ROW_NUMBER{8};
    static const uint8_t COLUMN_NUMBER{8};

    static void showBitBoard(uint64_t board)
    {
        // for (int8_t i = 0; i < ROW_NUMBER ; i++)
        for (int8_t i = ROW_NUMBER - 1; i >= 0; i--)
        {
            std::cout << std::bitset<ROW_NUMBER>{((board >> (i * ROW_NUMBER)) & ((unsigned int)std::pow(2, ROW_NUMBER) - 1))} << "\n";
        }
    }

    static const uint64_t setBit(const uint8_t &pos)
    {
        return 1 << pos;
    }
    static const uint8_t getIthRank(const uint64_t &table, const int &I)
    {
        return (table >> I) & 0xff;
    }
};
class FEN
{
public:
    FEN() = default;
    static const bool getTurn(const std::string &fen)
    {
        size_t firstSpace = fen.find(' ');
        return (fen[firstSpace + 1] == 'w' ? true : false);
    }

    static const std::bitset<4> getCastle(const std::string &fen)
    {
        size_t firstSpace = fen.find(' ');
        size_t secondSpace = fen.find(' ', firstSpace + 1);
        size_t thirdSpace = fen.find(' ', secondSpace + 1);
        std::string cat = fen.substr(secondSpace + 1, thirdSpace - secondSpace - 1);

        std::bitset<4> asd;
        asd = ((1 << 0) * (cat.find("K") != std::string::npos ? 1 : 0)) |
              ((1 << 1) * (cat.find("Q") != std::string::npos ? 1 : 0)) |
              ((1 << 2) * (cat.find("k") != std::string::npos ? 1 : 0)) |
              ((1 << 3) * (cat.find("q") != std::string::npos ? 1 : 0));
        return asd;
    }

    static const uint8_t getEnPassant(const std::string &fen)
    {
        size_t firstSpace = fen.find(' ');
        size_t secondSpace = fen.find(' ', firstSpace + 1);
        size_t thirdSpace = fen.find(' ', secondSpace + 1);
        size_t forthSpace = fen.find(' ', thirdSpace + 1);
        std::string cat = fen.substr(thirdSpace + 1, forthSpace - thirdSpace - 1);

        if (cat == "-")
            return 0;

        char column{cat[0]}, line{cat[1]};
        column = column - 'a';
        line = atoi(&line) - 1;

        return Utils::COLUMN_NUMBER * line + Utils::ROW_NUMBER - 1 - column;
    }

    static const uint8_t getHalfmoveClock(const std::string &fen)
    {
        size_t firstSpace = fen.find(' ');
        size_t secondSpace = fen.find(' ', firstSpace + 1);
        size_t thirdSpace = fen.find(' ', secondSpace + 1);
        size_t forthSpace = fen.find(' ', thirdSpace + 1);
        size_t fifthSpace = fen.find(' ', forthSpace + 1);
        std::string cat = fen.substr(forthSpace + 1, fifthSpace - forthSpace - 1);
        return stoi(cat);
    }

    static const uint16_t getFullmoveNumber(const std::string &fen)
    {
        size_t firstSpace = fen.find(' ');
        size_t secondSpace = fen.find(' ', firstSpace + 1);
        size_t thirdSpace = fen.find(' ', secondSpace + 1);
        size_t forthSpace = fen.find(' ', thirdSpace + 1);
        size_t fifthSpace = fen.find(' ', forthSpace + 1);
        size_t sixthSpace = fen.find(' ', fifthSpace + 1);
        std::string cat = fen.substr(fifthSpace + 1, sixthSpace - fifthSpace - 1);
        return stoi(cat);
    }

    /**
     * Parse a FEN string
     *
     * @param[in] value fen string
     * @param[in] value target piece (Ex. "k", "K", "Q", "q" etc.)
     * @return bitboard
     */
    static const uint64_t parsePiece(const std::string &fen, const char &piece)
    {
        uint64_t bitboard{0};
        uint8_t slashesMet{0}, columnIndex{0};
        for (const char &ch : fen)
        {
            if (ch == ' ')
                break;

            if (ch == '/')
            {
                ++slashesMet;
                columnIndex = 0;
            }
            else if (ch == piece)
            {
                uint64_t formula = (uint64_t)1 << (Utils::COLUMN_NUMBER * Utils::ROW_NUMBER - 1) - (Utils::ROW_NUMBER - 1 - columnIndex + Utils::ROW_NUMBER * slashesMet); // hell
                bitboard |= formula;
                ++columnIndex;
            }
            else if (std::isdigit(ch))
            {
                columnIndex += atoi(&ch);
            }
            else
            {
                ++columnIndex;
            }
        }
        return bitboard;
    }
};
