#ifndef CHESS_ENGINE_UCI_H
#define CHESS_ENGINE_UCI_H
#include "Logger.h"
#include <future>
#include <iostream>

class Uci {
    Game game;
    Board board;
    std::thread searchThread;

  public:
    Uci() : game(), board(), searchThread() {}

  public:
    void main() {
        std::string line;
        while (std::getline(std::cin, line)) {
            logger.log(line);
            std::stringstream ss(line);
            std::string command;
            ss >> command;
            this->processCommand(command, ss);
        }
    }

    void processCommand(std::string_view command, std::stringstream &ss) {
        if (command == "uci") {
            std::cout << "uciok" << std::endl;

        } else if (command == "isready") {
            std::cout << "readyok" << std::endl;

        } else if (command == "position") {
            std::string opponentMoveString;
            ss >> opponentMoveString; // discard startpos and moves
            if (!(ss >> opponentMoveString)) {
                board = Board{};
                return;
            }
            while (ss >> opponentMoveString)
                ;

            LegalMove legalMove{board};
            std::vector<std::shared_ptr<Move>> legalMoves;
            if (board.turn == WHITE) {
                legalMoves = legalMove.getLegalMoves<WHITE>().value();
            } else {
                legalMoves = legalMove.getLegalMoves<BLACK>().value();
            }
            auto opponentMove =
                std::find_if(legalMoves.begin(), legalMoves.end(), [&opponentMoveString](const std::shared_ptr<Move> &currentMove) {
                    return currentMove->toString() == opponentMoveString;
                });
            opponentMove->get()->makeMove(board);

        } else if (command == "go") {
            std::string opponentMoveString;
            ss >> opponentMoveString >> opponentMoveString; // wtime
            auto whiteTime = opponentMoveString;

            ss >> opponentMoveString >> opponentMoveString; // btime
            auto blackTime = opponentMoveString;

            ss >> opponentMoveString >> opponentMoveString; // btime
            auto movesToGo = opponentMoveString;

            if (board.turn == WHITE)
                game.time.allowedMilliseconds = std::stoi(whiteTime) / std::stoi(movesToGo);
            else
                game.time.allowedMilliseconds = std::stoi(blackTime) / std::stoi(movesToGo);

            //            game.time.allowedMilliseconds -= 5000;
            searchThread = std::thread{&Game::start, std::ref(game), std::ref(board)};
            searchThread.join();

            auto future = std::async(&Game::start, std::ref(game), std::ref(board));

            std::shared_ptr<Move> bestMove = future.get();

            std::cout << "bestmove " << *bestMove << std::endl;

            logger.log(std::format("bestmove {}\n", bestMove->toString()));

            bestMove->makeMove(board);

        } else if (command == "quit") {
            game.time.signalStop();
            if (searchThread.joinable())
                searchThread.join();
            exit(0);

        } else if (command == "stop") {
            game.time.signalStop();
            if (searchThread.joinable())
                searchThread.join();
        } else {
            std::cout << command << '\n';
        }
    }
};

#endif // CHESS_ENGINE_UCI_H
