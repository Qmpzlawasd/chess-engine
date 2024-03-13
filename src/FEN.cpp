#include "FEN.h"
#include "Castle.h"
#include "Colors.h"
#include "Utils.h"

Color FEN::parseTurn(const std::string &fen) {
    size_t firstSpace = fen.find(' ');
    if (fen[firstSpace + 1] == 'w') {
        return WHITE;
    }
    return BLACK;
}

template <Color side>
Castle<side> FEN::parseCastle(const std::string &fen) {
    size_t firstSpace = fen.find(' ');
    size_t secondSpace = fen.find(' ', firstSpace + 1);
    size_t thirdSpace = fen.find(' ', secondSpace + 1);
    std::string cat = fen.substr(secondSpace + 1, thirdSpace - secondSpace - 1);
    if constexpr (side == WHITE) {
        return Castle<WHITE>(cat.find('K') != std::string::npos, cat.find('Q') != std::string::npos);
    } else {
        return Castle<BLACK>(cat.find('k') != std::string::npos, cat.find('q') != std::string::npos);
    }
}

template Castle<WHITE> FEN::parseCastle<WHITE>(const std::string &);
template Castle<BLACK> FEN::parseCastle<BLACK>(const std::string &);

uint64_t FEN::parseEnPassant(const std::string &fen) {
    size_t firstSpace = fen.find(' ');
    size_t secondSpace = fen.find(' ', firstSpace + 1);
    size_t thirdSpace = fen.find(' ', secondSpace + 1);
    size_t forthSpace = fen.find(' ', thirdSpace + 1);
    std::string cat = fen.substr(thirdSpace + 1, forthSpace - thirdSpace - 1);

    if (cat == "-")
        return 0;

    const uint8_t number = cat[1] - '1';
    const uint8_t letter = cat[0] - 'a';

    return (uint64_t)1 << (Utils::ROW_NUMBER * number + letter);
}

uint8_t FEN::parseHalfmoveClock(const std::string &fen) {
    size_t firstSpace = fen.find(' ');
    size_t secondSpace = fen.find(' ', firstSpace + 1);
    size_t thirdSpace = fen.find(' ', secondSpace + 1);
    size_t forthSpace = fen.find(' ', thirdSpace + 1);
    size_t fifthSpace = fen.find(' ', forthSpace + 1);
    std::string cat = fen.substr(forthSpace + 1, fifthSpace - forthSpace - 1);
    uint8_t asd = stoi(cat);

    return asd;
}

uint16_t FEN::parseFullmoveNumber(const std::string &fen) {
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
uint64_t FEN::parsePiece(const std::string &fen, const char &piece) {
    uint64_t bitboard{0};
    uint8_t slashesMet{0}, columnIndex{0};
    for (const char &ch : fen) {
        if (ch == ' ')
            break;

        if (ch == '/') {
            ++slashesMet;
            columnIndex = 0;
        } else if (ch == piece) {
            const uint64_t formula = (uint64_t)1 << (Utils::COLUMN_NUMBER * Utils::ROW_NUMBER - slashesMet * Utils::ROW_NUMBER -
                                                     (Utils::ROW_NUMBER - columnIndex));
            bitboard |= formula;
            ++columnIndex;
        } else if (std::isdigit(ch)) {
            columnIndex += ch - '0';
        } else {
            ++columnIndex;
        }
    }
    return bitboard;
}
