#pragma once

#include <array>
#include <string>
#include <vector>
#include <sqlite3.h>

namespace campaign {

inline constexpr int startingBalance = 200;

struct Stage {
    const char* title;
    const char* location;
    int players;
    int smallBlind;
    int opponentStack;
};

inline constexpr std::array<Stage, 4> stages{{
    {"Bratislava Pub Series", "Old Town pub, Bratislava", 4, 2, 200},
    {"European Poker Tour", "Barcelona, Spain", 6, 10, 800},
    {"World Poker Tour", "Prague, Czech Republic", 8, 25, 3000},
    {"World Series of Poker", "Las Vegas, USA", 9, 100, 12000}
}};

struct Profile {
    int balance = startingBalance;
    int completedStages = 0;

    bool canEnter(int stage) const;
    void recordResult(int stage, int remainingBalance, bool seriesWon);
};

enum class SeriesResult { InProgress, Won, Lost };
SeriesResult seriesResult(const std::vector<int>& stacks, std::size_t humanIndex);

// Registered players use users.balance; the profile balance also supports guests.
class Store {
public:
    explicit Store(sqlite3* database) : db(database) {}
    bool initialize();
    bool load(const std::string& username, Profile& profile);
    bool save(const std::string& username, const Profile& profile);
private:
    sqlite3* db;
};

} // namespace campaign
