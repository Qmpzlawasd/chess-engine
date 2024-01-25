#pragma once

class Piece {

    uint64_t white;
    uint64_t black;

  public:
    Piece(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : white{whiteConfiguration}, black{blackConfiguration} {};
    const uint64_t getWhite() const { return white; }
    const uint64_t getBlack() const { return black; }
};

class Pawn : public Piece {
  public:
    Pawn(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : Piece{whiteConfiguration, blackConfiguration} {};
};

class Queen : public Piece {
  public:
    Queen(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : Piece{whiteConfiguration, blackConfiguration} {};
};

class Rook : public Piece {
  public:
    Rook(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : Piece{whiteConfiguration, blackConfiguration} {};
    static const uint64_t getNaiveAttackPattern(const Square &square) {
        const uint8_t line = square % Utils::ROW_NUMBER;
        const uint8_t column = square / Utils::COLUMN_NUMBER;

        const uint64_t PIECE_FILE = Utils::A_FILE << line;
        const uint64_t PIECE_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * column;

        return (PIECE_FILE | PIECE_ROW) & ~Utils::setSquare(square);
    }
};

class Bishop : public Piece {
  public:
    Bishop(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : Piece{whiteConfiguration, blackConfiguration} {};
    static const uint64_t getNaiveAttackPattern(const Square &square) {
        const uint8_t line = square % Utils::ROW_NUMBER;
        const uint8_t column = square / Utils::COLUMN_NUMBER;

        uint64_t naiveAttackPattern = 0;

        for (uint8_t i = line, j = column;
             i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0;
             i++, j++) {
            naiveAttackPattern |= Utils::setSquare(Square(j * Utils::COLUMN_NUMBER + i));
        }
        for (uint8_t i = line, j = column;
             i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0;
             i++, j--) {
            naiveAttackPattern |= Utils::setSquare(Square(j * Utils::COLUMN_NUMBER + i));
        }
        for (uint8_t i = line, j = column;
             i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0;
             i--, j++) {
            naiveAttackPattern |= Utils::setSquare(Square(j * Utils::COLUMN_NUMBER + i));
        }
        for (uint8_t i = line, j = column;
             i < Utils::ROW_NUMBER and j < Utils::COLUMN_NUMBER and j >= 0 and i >= 0;
             i--, j--) {
            naiveAttackPattern |= Utils::setSquare(Square(j * Utils::COLUMN_NUMBER + i));
        }
        return naiveAttackPattern & ~Utils::setSquare(square);
    }
};

class King : public Piece {
  public:
    King(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : Piece{whiteConfiguration, blackConfiguration} {};
};

class Knight : public Piece {
  public:
    Knight(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : Piece{whiteConfiguration, blackConfiguration} {};
};
