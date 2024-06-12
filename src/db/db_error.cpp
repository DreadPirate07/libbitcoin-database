#include "bitcoin/database-sqlite/wrapper/db.hpp"
#include "bitcoin/database-sqlite/wrapper/db_error.hpp"


namespace db {
    db_error::db_error(char const* msg) : std::runtime_error(msg) {}
    db_error::db_error(database& db) : std::runtime_error(sqlite3_errmsg(db.db_)) {}
}