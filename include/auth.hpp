#pragma once

#include <sqlite3.h>
#include <string>

namespace auth {

bool createUsersTable(sqlite3* db);

bool userExists(sqlite3* db,
                const std::string& username,
                const std::string& email);

bool registerUser(sqlite3* db,
                  const std::string& username,
                  const std::string& email,
                  const std::string& plainPassword);

bool loginUser(sqlite3* db,
               const std::string& usernameOrEmail,
               const std::string& plainPassword);

}
