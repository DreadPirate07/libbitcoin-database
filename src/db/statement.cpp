#include "bitcoin/database-sqlite/wrapper/statement.hpp"
#include "bitcoin/database-sqlite/wrapper/db_error.hpp"
#include <cstring>
#include <memory>


namespace db {
    statement::statement(database& db, char const* stmt) : db(db), stmt(0), tail(0)
    {
        if (stmt) {
        auto rc = prepare(stmt);
        if (rc != SQLITE_OK)
            throw db_error(db);
        }
    }

    statement::~statement()
    {
        // finalize() can return error. If you want to check the error, call
        // finalize() explicitly before this object is destructed.
        finalize();
    }

    int statement::prepare(char const* stmt)
    {
        auto rc = finalize();
        if (rc != SQLITE_OK)
        return rc;

        return prepare_impl(stmt);
    }

    int statement::prepare_impl(char const* statement)
    {
        return sqlite3_prepare_v2(db.db_, statement, std::strlen(statement), &stmt, &tail);
    }

    int statement::finalize()
    {
        auto rc = SQLITE_OK;
        if (stmt) {
        rc = finalize_impl(stmt);
        stmt = nullptr;
        }
        tail = nullptr;

        return rc;
    }

    int statement::finalize_impl(sqlite3_stmt* stmt)
    {
        return sqlite3_finalize(stmt);
    }

    int statement::step()
    {
        return sqlite3_step(stmt);
    }

    int statement::reset()
    {
        return sqlite3_reset(stmt);
    }

    int statement::bind(int idx, int value)
    {
        return sqlite3_bind_int(stmt, idx, value);
    }

    int statement::bind(int idx, double value)
    {
        return sqlite3_bind_double(stmt, idx, value);
    }

    int statement::bind(int idx, long long int value)
    {
        return sqlite3_bind_int64(stmt, idx, value);
    }

    int statement::bind(int idx, char const* value, copy_semantic fcopy)
    {
        return sqlite3_bind_text(stmt, idx, value, std::strlen(value), fcopy == copy ? SQLITE_TRANSIENT : SQLITE_STATIC );
    }

    int statement::bind(int idx, void const* value, int n, copy_semantic fcopy)
    {
        return sqlite3_bind_blob(stmt, idx, value, n, fcopy == copy ? SQLITE_TRANSIENT : SQLITE_STATIC );
    }

    int statement::bind(int idx, std::string const& value, copy_semantic fcopy)
    {
        return sqlite3_bind_text(stmt, idx, value.c_str(), value.size(), fcopy == copy ? SQLITE_TRANSIENT : SQLITE_STATIC );
    }

    int statement::bind(int idx)
    {
        return sqlite3_bind_null(stmt, idx);
    }

    int statement::bind(int idx, null_type)
    {
        return bind(idx);
    }

    int statement::bind(char const* name, int value)
    {
        auto idx = sqlite3_bind_parameter_index(stmt, name);
        return bind(idx, value);
    }

    int statement::bind(char const* name, double value)
    {
        auto idx = sqlite3_bind_parameter_index(stmt, name);
        return bind(idx, value);
    }

    int statement::bind(char const* name, long long int value)
    {
        auto idx = sqlite3_bind_parameter_index(stmt, name);
        return bind(idx, value);
    }

    int statement::bind(char const* name, char const* value, copy_semantic fcopy)
    {
        auto idx = sqlite3_bind_parameter_index(stmt, name);
        return bind(idx, value, fcopy);
    }

    int statement::bind(char const* name, void const* value, int n, copy_semantic fcopy)
    {
        auto idx = sqlite3_bind_parameter_index(stmt, name);
        return bind(idx, value, n, fcopy);
    }

    int statement::bind(char const* name, std::string const& value, copy_semantic fcopy)
    {
        auto idx = sqlite3_bind_parameter_index(stmt, name);
        return bind(idx, value, fcopy);
    }

    int statement::bind(char const* name)
    {
        auto idx = sqlite3_bind_parameter_index(stmt, name);
        return bind(idx);
    }

    int statement::bind(char const* name, null_type)
    {
        return bind(name);
    }
}