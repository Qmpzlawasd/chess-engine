#ifndef CHESS_ENGINE_LOGGER_H
#define CHESS_ENGINE_LOGGER_H

#include <fstream>
#include <ostream>

class Logger {
    std::ofstream outFile{"log/Engine.log"};

  public:
    void log(const auto &res, std::string_view term = "\n") {
        outFile << res << term;
        outFile.flush();
    }
};

static Logger logger;

#endif // CHESS_ENGINE_LOGGER_H
