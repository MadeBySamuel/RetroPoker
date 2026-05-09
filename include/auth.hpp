#pragma once

#include <sqlite3.h>
#include <string>

namespace auth {




struct LoginResult {
    enum class Status{
        Pass, 
        NotFound, 
        WrongPassword,
        DatabaseError,
        UserAlreadyExist,
        Wrong,
        Empty
    };
    Status status;
    std::string username;
};

bool createUsersTable(sqlite3* db);

LoginResult userExists(sqlite3* db,
                const std::string& username,
                const std::string& email);

LoginResult registerUser(sqlite3* db,
                  const std::string& username,
                  const std::string& email,
                  const std::string& plainPassword);

LoginResult loginUser(sqlite3* db,
               const std::string& usernameOrEmail,
               const std::string& plainPassword);

}
