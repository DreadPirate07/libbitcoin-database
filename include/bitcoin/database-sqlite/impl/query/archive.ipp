#ifndef LIBBITCOIN_DATABASE_QUERY_ARCHIVE_IPP
#define LIBBITCOIN_DATABASE_QUERY_ARCHIVE_IPP

#include <sqlite3.h>
#include <bitcoin/system.hpp>
#include <bitcoin/system/define.hpp>
#include "include/bitcoin/database-sqlite/store/table.hpp"
#include "include/bitcoin/database-sqlite/store/schema.hpp"
#include "include/bitcoin/database-sqlite/wrapper/statement.hpp"
#include "include/bitcoin/database-sqlite/wrapper/db.hpp"

namespace libbitcoin {
namespace database {

TEMPLATE
inline bool CLASS::is_header(const hash_digest& key) const NOEXCEPT
{
    const char* sql = "SELECT 1 FROM headers WHERE hash = ? LIMIT 1";
    sqlite::db::statement stmt(store_.db);
    if (stmt.prepare(sql) != SQLITE_OK) return false;
    stmt.bind(1, encode_hash(key), db::copy);
    return stmt.step() == SQLITE_ROW;
}

// this function still needs to be implemented!
TEMPLATE
inline bool CLASS::is_block(const hash_digest& key) const NOEXCEPT
{
    return is_associated(to_header(key));
}

TEMPLATE
inline bool CLASS::is_tx(const hash_digest& key) const NOEXCEPT
{
    const char* sql = "SELECT 1 FROM transactions WHERE hash = ? LIMIT 1";
    sqlite::db::statement stmt(store_.db);
    if (stmt.prepare(sql) != SQLITE_OK) return false;
    stmt.bind(1, encode_hash(key), db::copy);
    return stmt.step() == SQLITE_ROW;
}

TEMPLATE
inline bool CLASS::is_coinbase(const tx_link& link) const NOEXCEPT
{
    const char* sql = "SELECT is_coinbase FROM transactions WHERE rowid = ?";
    sqlite::db::statement stmt(store_.db);
    if (stmt.prepare(sql) != SQLITE_OK) return false;
    stmt.bind(1, link.value());
    if (stmt.step() != SQLITE_ROW) return false;
    return stmt.column_int(0) != 0;
}

TEMPLATE
inline bool CLASS::is_milestone(const header_link& link) const NOEXCEPT
{
    const char* sql = "SELECT is_milestone FROM headers WHERE rowid = ?";
    sqlite::db::statement stmt(store_.db);
    if (stmt.prepare(sql) != SQLITE_OK) return false;
    stmt.bind(1, link.value());
    if (stmt.step() != SQLITE_ROW) return false;
    return stmt.column_int(0) != 0;
}

TEMPLATE
inline bool CLASS::is_associated(const header_link& link) const NOEXCEPT
{
    const char* sql = "SELECT is_associated FROM txs WHERE header_link = ?";
    sqlite::db::statement stmt(store_.db);
    if (stmt.prepare(sql) != SQLITE_OK) return false;
    stmt.bind(1, link.value());
    if (stmt.step() != SQLITE_ROW) return false;
    return stmt.column_int(0) != 0;
}
}
}

#endif