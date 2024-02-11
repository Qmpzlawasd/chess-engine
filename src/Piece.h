#ifndef CHESS_ENGINE_PIECE_H
#define CHESS_ENGINE_PIECE_H

#include "Squares.h"
#include "Utils.h"
#include <array>

class Piece {
    uint64_t white;
    uint64_t black;

  public:
    Piece() : white{0}, black{0} {};

    Piece(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : white{whiteConfiguration}, black{blackConfiguration} {};

    [[nodiscard]] uint64_t getWhite() const { return white; }

    [[nodiscard]] uint64_t getBlack() const { return black; }
};

class Pawn : public Piece {
  public:
    Pawn(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
};

class King : public Piece {
  public:
    King(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
};

class Knight : public Piece {
  public:
    Knight(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece{whiteConfiguration, blackConfiguration} {};
};

class SlidingPiece : public Piece {
  public:
    SlidingPiece() : Piece(){};

    SlidingPiece(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : Piece(whiteConfiguration, blackConfiguration){};

    [[nodiscard]] virtual uint8_t getShiftValue() const noexcept = 0;
    [[nodiscard]] virtual uint64_t getNaiveAttackPattern(const Square &square) const noexcept = 0;
    [[nodiscard]] virtual uint64_t fillPositions(const Square &, const uint64_t &) const noexcept = 0;
    [[nodiscard]] virtual const std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE> &getMagicConstants() const noexcept = 0;
};

class Rook : public SlidingPiece {
  public:
    static constexpr uint8_t SHIFT_VALUE = 48;
    static constexpr std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE> MAGIC_CONSTANTS = {
        140741818375707,   281488098640071,  281487982064037,  281487887622693,  281487875079702,   281505413132412,   281487966569754,
        285657865961369,   284982915493826,  145837244596824,  85343154652921,   8885188538603839,  22330347429612222, 10186082776017060,
        11499801072313598, 8888020683702941, 8899865402692176, 9083623976061481, 23173016471178211, 13843467313434968, 18835453908674568,
        42725395419524,    517610615100058,  19536873022003,   282145542187802,  148928847946864,   75146771372192,    39378117458573,
        28162100505257,    9170268013514,    642383811335262,  18848688231567,   281801449792584,   144950729355539,   73699832211488,
        51270148685609,    49119214527935,   37839289602716,   26187780672737,   18845191200365,    283177819373456,   141014625054515,
        71006743560122,    44974150185680,   23224510157669,   36046956394214,   22265219041766,    25227631865993,    281655872240761,
        141396953657003,   512713915629648,  37809518113178,   69540553661822,   19098025015652,    241542639552078,   19351592501501,
        281523529011831,   145739376129651,  72421227302431,   87720104566739,   74221095215879,    37059116532335,    20066245937723,
        19470701407538,
    };

    Rook() : SlidingPiece{} {};

    Rook(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : SlidingPiece{whiteConfiguration, blackConfiguration} {};

    [[nodiscard]] uint64_t getNaiveAttackPattern(const Square &square) const noexcept override;
    [[nodiscard]] uint64_t fillPositions(const Square &square, const uint64_t &pattern) const noexcept override;
    [[nodiscard]] uint8_t getShiftValue() const noexcept override { return Rook::SHIFT_VALUE; }
    [[nodiscard]] const std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE> &getMagicConstants() const noexcept override {
        return Rook::MAGIC_CONSTANTS;
    }
};

class Bishop : public SlidingPiece {
  public:
    static constexpr uint8_t SHIFT_VALUE = 48;
    static constexpr std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE> MAGIC_CONSTANTS = {
        636508561067425083, 636508561067425083, 636508561067425083, 636508561067425083,  636508561067425083, 636508561067425083,
        636508561067425083, 636508561067425083, 636508561067425083, 636508561067425083,  636508561067425083, 636508561067425083,
        636508561067425083, 636508570080256910, 636508570080256910, 636508561067425083,  636508561067425083, 636508561067425083,
        636508993683352779, 636508606131584218, 636508588105920564, 636508561067425083,  636508561067425083, 636508561067425083,
        636508561067425083, 636508561067425083, 636508633170079699, 1369097138574082643, 3095950409052152,   636508579093088737,
        636508579093088737, 636508561067425083, 636508561067425083, 636508561067425083,  636508588105920564, 641123680745590530,
        638243693425095469, 636508768362557104, 636508570080256910, 636508561067425083,  636508561067425083, 636508561067425083,
        636508696259902488, 636508588105920564, 636508615144416045, 636508615144416045,  636508561067425083, 636508561067425083,
        636508561067425083, 636508561067425083, 636508561067425083, 636508561067425083,  636508561067425083, 636508561067425083,
        636508561067425083, 636508561067425083, 636508561067425083, 636508561067425083,  636508561067425083, 636508561067425083,
        636508561067425083, 636508561067425083, 636508561067425083, 636508561067425083,
    };
    Bishop() : SlidingPiece{} {};

    Bishop(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration)
        : SlidingPiece{whiteConfiguration, blackConfiguration} {};

    [[nodiscard]] uint64_t getNaiveAttackPattern(const Square &square) const noexcept override;

    [[nodiscard]] uint64_t fillPositions(const Square &square, const uint64_t &pattern) const noexcept override;
    [[nodiscard]] uint8_t getShiftValue() const noexcept override { return Bishop::SHIFT_VALUE; }
    [[nodiscard]] const std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE> &getMagicConstants() const noexcept override {
        return Bishop::MAGIC_CONSTANTS;
    }
};

class Queen : public SlidingPiece {
  public:
    Queen() : SlidingPiece{} {};

    Queen(const uint64_t &whiteConfiguration, const uint64_t &blackConfiguration) : SlidingPiece{whiteConfiguration, blackConfiguration} {};

    [[nodiscard]] uint64_t getNaiveAttackPattern(const Square &square) const noexcept override;
    [[nodiscard]] uint64_t fillPositions(const Square &square, const uint64_t &pattern) const noexcept override;
    [[nodiscard]] uint8_t getShiftValue() const noexcept override { return Rook::SHIFT_VALUE; }
    [[nodiscard]] const std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE> &getMagicConstants() const noexcept override {
        return Rook::MAGIC_CONSTANTS;
    }
};

#endif // CHESS_ENGINE_PIECE_H
