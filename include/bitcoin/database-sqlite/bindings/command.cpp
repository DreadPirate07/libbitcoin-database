#include "command.hpp"
#include "statement.hpp"
#include "db.hpp"
#include <string>
#include <string.h>
#include <cstring>

namespace db
{
    command::bindstream::bindstream(command& cmd, int idx) : cmd_(cmd), idx_(idx){}

    command::command(database& db, char const* stmt) : statement(db, stmt){}

    command::bindstream command::binder(int idx)
    {
        return bindstream(*this, idx);
    }

    int command::execute()
    {
        auto rc = step();
        if (rc == SQLITE_DONE) rc = SQLITE_OK;

        return rc;
    }

    int command::execute_all()
    {
        auto rc = execute();
        if (rc != SQLITE_OK) return rc;

        char const* sql = tail;

        while (std::strlen(sql) > 0) { // sqlite3_complete() is broken.
        sqlite3_stmt* old_stmt = stmt;

        if ((rc = prepare_impl(sql)) != SQLITE_OK) return rc;

        if ((rc = sqlite3_transfer_bindings(old_stmt, stmt)) != SQLITE_OK) return rc;

        finalize_impl(old_stmt);

        if ((rc = execute()) != SQLITE_OK) return rc;

        sql = tail;
        }

        return rc;
    }
        command::bindstream::bindstream(command& cmd, int idx) : cmd_(cmd), idx_(idx){}

    command::command(database& db, char const* stmt) : statement(db, stmt){}

    command::bindstream command::binder(int idx)
    {
        return bindstream(*this, idx);
    }

    int command::execute()
    {
        auto rc = step();
        if (rc == SQLITE_DONE) rc = SQLITE_OK;

        return rc;
    }

    int command::execute_all()
    {
        auto rc = execute();
        if (rc != SQLITE_OK) return rc;

        char const* sql = tail;

        while (std::strlen(sql) > 0) { // sqlite3_complete() is broken.
        sqlite3_stmt* old_stmt = stmt;

        if ((rc = prepare_impl(sql)) != SQLITE_OK) return rc;

        if ((rc = sqlite3_transfer_bindings(old_stmt, stmt)) != SQLITE_OK) return rc;

        finalize_impl(old_stmt);

        if ((rc = execute()) != SQLITE_OK) return rc;

        sql = tail;
        }
        return rc;
    }
} // namespace db

