#pragma once

class Utils
{
private:
    static void showReversedBin(int8_t nr)
    {
        for (size_t i = 0; i < 8; i++)
        {
            std::cout << ((nr >> i) & 1) << "-";
        }
        std::cout << "\n";
    }

public:
    static const bool WHITE = true;
    static const bool BLACK = false;

    static const uint8_t ROW_NUMBER{8};
    static const uint8_t COLUMN_NUMBER{8};

    static const uint64_t A_FILE = 0x0101010101010101;
    static const uint64_t H_FILE = 0x8080808080808080;
    static const uint64_t FIRST_ROW = 0x00000000000000ff;

    static void showBitBoard(const uint64_t &board)
    {
        // for (int8_t i = 0; i < ROW_NUMBER ; i++)
        for (int8_t i = ROW_NUMBER - 1; i >= 0; i--)
        {
            showReversedBin((board >> (i * ROW_NUMBER)) & ((unsigned int)(1 << ROW_NUMBER) - 1));
        }
        printf("\n");
    }

    /**
     * @param[in] value bitboard
     * @param[in] value in rage from 0 to 7
     */
    static const uint64_t getIthFile(const uint64_t &table, const int &I)
    {
        return table & A_FILE >> I;
    }

    /**
     * @param[in] value bitboard
     * @param[in] value in rage from 0 to 7
     */
    static const uint64_t getIthRank(const uint64_t &table, const int &I)
    {
        return table & FIRST_ROW << I * 8;
    }
};
class FEN
{
public:
    FEN() = default;
    static const bool parseTurn(const std::string &fen)
    {
        size_t firstSpace = fen.find(' ');
        return (fen[firstSpace + 1] == 'w' ? true : false);
    }

    static const std::bitset<4> parseCastle(const std::string &fen)
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

    static const uint64_t parseEnPassant(const std::string &fen)
    {
        size_t firstSpace = fen.find(' ');
        size_t secondSpace = fen.find(' ', firstSpace + 1);
        size_t thirdSpace = fen.find(' ', secondSpace + 1);
        size_t forthSpace = fen.find(' ', thirdSpace + 1);
        std::string cat = fen.substr(thirdSpace + 1, forthSpace - thirdSpace - 1);

        if (cat == "-")
            return 0;

        const uint8_t number = cat[1] - '1';
        const uint8_t letter = cat[0] - 'a';

        return (uint64_t)1 << Utils::ROW_NUMBER * number + letter;
    }

    static const uint8_t parseHalfmoveClock(const std::string &fen)
    {
        size_t firstSpace = fen.find(' ');
        size_t secondSpace = fen.find(' ', firstSpace + 1);
        size_t thirdSpace = fen.find(' ', secondSpace + 1);
        size_t forthSpace = fen.find(' ', thirdSpace + 1);
        size_t fifthSpace = fen.find(' ', forthSpace + 1);
        std::string cat = fen.substr(forthSpace + 1, fifthSpace - forthSpace - 1);
        std::cout << stoi(cat);
        uint8_t asd = stoi(cat);
        std::cout << asd;

        return asd;
    }

    static const uint16_t parseFullmoveNumber(const std::string &fen)
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
                const uint64_t formula = (uint64_t)1 << Utils::COLUMN_NUMBER * Utils::ROW_NUMBER - slashesMet * Utils::ROW_NUMBER - (Utils::ROW_NUMBER - columnIndex);
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
