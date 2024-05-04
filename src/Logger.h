#ifndef CHESS_ENGINE_LOGGER_H
#define CHESS_ENGINE_LOGGER_H

#include <fstream>
#include <ostream>

class Logger {
    std::ofstream f{"/home/stefan/Engine.log"};

  public:
    void log(auto res) { f << res << std::endl; }
};

static Logger logger;

#endif // CHESS_ENGINE_LOGGER_H
