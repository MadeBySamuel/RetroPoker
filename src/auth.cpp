
#include "auth.hpp"

#include <argon2.h>

#include <array>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>


namespace auth {
namespace {

constexpr std::uint32_t kTimeCost = 2;
constexpr std::uint32_t kMemoryCostKiB = 19456;
constexpr std::uint32_t kParallelism = 1;
constexpr std::size_t kSaltLen = 16;
constexpr std::size_t kHashLen = 32;

bool generateSalt(std::array<std::uint8_t, kSaltLen>& salt) {
    std::ifstream urandom("/dev/urandom", std::ios::binary);
    if (!urandom) {
        return false;
    }

    urandom.read(reinterpret_cast<char*>(salt.data()), static_cast<std::streamsize>(salt.size()));

    return urandom.gcount() == static_cast<std::streamsize>(salt.size());
}

bool hashPassword(const std::string& plainPassword, std::string& outHash) {
    std::array<std::uint8_t, kSaltLen> salt{};
    if (!generateSalt(salt)) {
        return false;
    }

    const std::size_t encodedLen = argon2_encodedlen(kTimeCost, kMemoryCostKiB, kParallelism, kSaltLen, kHashLen, Argon2_id);

    std::vector<char> encoded(encodedLen);

    const int rc = argon2id_hash_encoded(
        kTimeCost,
        kMemoryCostKiB,
        kParallelism,
        plainPassword.data(),
        plainPassword.size(),
        salt.data(),
        salt.size(),
        kHashLen,
        encoded.data(),
        encoded.size()
    );

    if (rc != ARGON2_OK) {
        return false;
    }

    outHash = encoded.data();
    return true;
}

bool verifyPassword(const std::string& plainPassword,const std::string& storedHash) {
    if (argon2id_verify(storedHash.c_str(), plainPassword.data(),plainPassword.size()) == ARGON2_OK){
        return 1;
    }
    else {
        return 0;
    }
}

} // namespace


bool createUsersTable(sqlite3* db) {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL UNIQUE,"
        "email TEXT NOT NULL UNIQUE,"
        "password_hash TEXT NOT NULL,"
        "balance INTEGER NOT NULL DEFAULT 1000,"
        "created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,"
        "last_login_at TEXT"
        ");";

    return sqlite3_exec(db, sql, nullptr, nullptr, nullptr) == SQLITE_OK;
}

LoginResult userExists(sqlite3* db,const std::string& username, const std::string& email) {
    const char* sql = "SELECT 1 FROM users WHERE username = ?1 OR email = ?2 LIMIT 1;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return {LoginResult::Status::DatabaseError};
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);

    const int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return {LoginResult::Status::Pass};

}

LoginResult registerUser(sqlite3* db, const std::string& username, const std::string& email, const std::string& plainPassword) {
    
    if (username.empty() || email.empty() || plainPassword.empty()) {
        return {LoginResult::Status::Empty};
    }


    auto user = userExists(db, username, email);

    if (user.status == LoginResult::Status::UserAlreadyExist) {
        return {LoginResult::Status::UserAlreadyExist};
    }

    std::string passwordHash;

    if (!hashPassword(plainPassword, passwordHash)) {
        return {LoginResult::Status::DatabaseError};
    }


    const char* sql =
        "INSERT INTO users (username, email, password_hash) "
        "VALUES (?1, ?2, ?3);";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return {LoginResult::Status::DatabaseError};
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, passwordHash.c_str(), -1, SQLITE_TRANSIENT);

    const int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return {LoginResult::Status::Pass};
}

LoginResult loginUser(sqlite3* db, const std::string& usernameOrEmail, const std::string& plainPassword) {
    
    const char* sql =
        "SELECT username, password_hash "
        "FROM users "
        "WHERE username = ?1 "
        "LIMIT 1;";

    sqlite3_stmt* stmt;


    // služi na skompilovanie sql 
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, usernameOrEmail.c_str(), -1, SQLITE_TRANSIENT);


    std::cout << "hladam: " << usernameOrEmail << "\n";


    const int rc = sqlite3_step(stmt);

    std::cout << "rc: " << rc << "\n"; // 100 = SQLITE_ROW, 101 = SQLITE_DONE


    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return {LoginResult::Status::NotFound};
    }

    const unsigned char* rawName = sqlite3_column_text(stmt, 0);
    const unsigned char* rawHash = sqlite3_column_text(stmt, 1);

    const std::string username = rawName ? reinterpret_cast<const char*>(rawName) : "";
    const std::string storedHash = rawHash ? reinterpret_cast<const char*>(rawHash) : "";

    
    sqlite3_finalize(stmt);

    if (storedHash.empty()){
    return {LoginResult::Status::DatabaseError};
    }
    
    if (!verifyPassword(plainPassword, storedHash)){
        return {LoginResult::Status::WrongPassword};
    }

    
    return {LoginResult::Status::Pass,username};
}



}


