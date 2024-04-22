#ifndef CHESS_ENGINE_TIME_H
#define CHESS_ENGINE_TIME_H

#include <chrono>
class Time {
    std::chrono::time_point<std::chrono::steady_clock> startTime;

  private:
    template <class result_t = std::chrono::milliseconds,
              class clock_t = std::chrono::steady_clock,
              class duration_t = std::chrono::milliseconds>
    auto getElapsedMs() noexcept {
        return std::chrono::duration_cast<result_t>(clock_t::now() - startTime);
    }

  public:
    explicit Time() : startTime() {}

    void start() noexcept { startTime = std::chrono::steady_clock::now(); }

    static constexpr float allowedMilliseconds = 6000;

    bool checkTimeIsUp() noexcept {
        if (getElapsedMs().count() >= allowedMilliseconds) {
            puts("TimesUP");
            return true;
        }
        return false;
    }
};
#endif // CHESS_ENGINE_TIME_H
