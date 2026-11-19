#include "campaign.hpp"

#include <algorithm>

namespace campaign {

bool Profile::canEnter(int stage) const {
    return balance > 0 && stage >= 0 && stage < static_cast<int>(stages.size()) && stage <= completedStages;
}

void Profile::recordResult(int stage, int remainingBalance, bool seriesWon) {
    if (stage < 0 || stage >= static_cast<int>(stages.size()) || stage > completedStages) return;
    balance = std::max(0, remainingBalance);
    if (seriesWon && balance > 0 && stage == completedStages) {
        ++completedStages;
    }
}

SeriesResult seriesResult(const std::vector<int>& stacks, std::size_t humanIndex) {
    if (humanIndex >= stacks.size()) return SeriesResult::InProgress;
    if (stacks[humanIndex] <= 0) return SeriesResult::Lost;
    for (std::size_t i = 0; i < stacks.size(); ++i) {
        if (i != humanIndex && stacks[i] > 0) return SeriesResult::InProgress;
    }
    return SeriesResult::Won;
}

bool Store::initialize() {
    return db && sqlite3_exec(db,
        "CREATE TABLE IF NOT EXISTS campaign_profiles ("
        "username TEXT PRIMARY KEY, balance INTEGER NOT NULL CHECK(balance >= 0),"
        "completed_stages INTEGER NOT NULL DEFAULT 0 CHECK(completed_stages BETWEEN 0 AND 4));",
        nullptr, nullptr, nullptr) == SQLITE_OK;
}

bool Store::load(const std::string& username, Profile& profile) {
    sqlite3_stmt* statement = nullptr;
    const char* sql =
        "SELECT COALESCE(u.balance, c.balance, 200), COALESCE(c.completed_stages, 0) "
        "FROM (SELECT ?1 AS username) p "
        "LEFT JOIN campaign_profiles c ON c.username = p.username "
        "LEFT JOIN users u ON u.username = p.username AND p.username <> '';";
    if (sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_text(statement, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    const bool loaded = sqlite3_step(statement) == SQLITE_ROW;
    if (loaded) {
        profile.balance = std::max(0, sqlite3_column_int(statement, 0));
        profile.completedStages = std::clamp(sqlite3_column_int(statement, 1), 0,
                                           static_cast<int>(stages.size()));
    }
    sqlite3_finalize(statement);
    return loaded;
}

bool Store::save(const std::string& username, const Profile& profile) {
    if (profile.balance < 0 || profile.completedStages < 0 ||
        profile.completedStages > static_cast<int>(stages.size())) return false;
    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) return false;

    const auto write = [&](const char* sql, bool progress) {
        sqlite3_stmt* statement = nullptr;
        if (sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) != SQLITE_OK) return false;
        sqlite3_bind_text(statement, 1, username.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(statement, 2, profile.balance);
        if (progress) sqlite3_bind_int(statement, 3, profile.completedStages);
        const bool saved = sqlite3_step(statement) == SQLITE_DONE;
        sqlite3_finalize(statement);
        return saved;
    };

    bool saved = write(
        "INSERT INTO campaign_profiles(username, balance, completed_stages) VALUES(?1, ?2, ?3) "
        "ON CONFLICT(username) DO UPDATE SET balance = excluded.balance, "
        "completed_stages = excluded.completed_stages;", true);
    if (saved && !username.empty()) {
        saved = write("UPDATE users SET balance = ?2 WHERE username = ?1;", false);
    }
    if (saved && sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) == SQLITE_OK) return true;
    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    return false;
}

} 
