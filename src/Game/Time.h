#ifndef CHESS_ENGINE_TIME_H
#define CHESS_ENGINE_TIME_H

#include <chrono>
class Time {
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    bool forceStop;

  private:
    template <class result_t = std::chrono::milliseconds,
              class clock_t = std::chrono::steady_clock,
              class duration_t = std::chrono::milliseconds>
    auto getElapsedMs() noexcept {
        return std::chrono::duration_cast<result_t>(clock_t::now() - startTime);
    }

  public:
    float allowedMilliseconds = 6000;

    explicit Time() : startTime{}, forceStop{false} {}

    void signalStop() noexcept { forceStop = true; }

    void start() noexcept { startTime = std::chrono::steady_clock::now(); }

    bool checkTimeIsUp() noexcept {
        if (forceStop) {
            forceStop = false;
            return true;
        }

        if (getElapsedMs().count() >= allowedMilliseconds) {
            return true;
        }
        return false;
    }
    unsigned long stop() noexcept { return static_cast<unsigned long>(getElapsedMs().count()); }
};
#endif // CHESS_ENGINE_TIME_H
