#include "db_query.hpp"
#include "db_error.hpp"


namespace db {
    query::rows::getstream::getstream(rows* rws, int idx) : rws_(rws), idx_(idx){}

    query::rows::rows(sqlite3_stmt* stmt) : stmt_(stmt) {}

    int query::rows::data_count() const
    {
        return sqlite3_data_count(stmt_);
    }

    int query::rows::column_type(int idx) const
    {
        return sqlite3_column_type(stmt_, idx);
    }

    int query::rows::column_bytes(int idx) const
    {
        return sqlite3_column_bytes(stmt_, idx);
    }

    int query::rows::get(int idx, int) const
    {
        return sqlite3_column_int(stmt_, idx);
    }

    double query::rows::get(int idx, double) const
    {
        return sqlite3_column_double(stmt_, idx);
    }

    long long int query::rows::get(int idx, long long int) const
    {
        return sqlite3_column_int64(stmt_, idx);
    }

    char const* query::rows::get(int idx, char const*) const
    {
        return reinterpret_cast<char const*>(sqlite3_column_text(stmt_, idx));
    }

    std::string query::rows::get(int idx, std::string) const
    {
        return get(idx, (char const*)0);
    }

    void const* query::rows::get(int idx, void const*) const
    {
        return sqlite3_column_blob(stmt_, idx);
    }

    null_type query::rows::get(int /*idx*/, null_type) const
    {
        return ignore;
    }
    query::rows::getstream query::rows::getter(int idx)
    {
        return getstream(this, idx);
    }

    query::query_iterator::query_iterator() : cmd_(0)
    {
        rc_ = SQLITE_DONE;
    }

    query::query_iterator::query_iterator(query* cmd) : cmd_(cmd)
    {
        rc_ = cmd_->step();
        if (rc_ != SQLITE_ROW && rc_ != SQLITE_DONE)
        throw db_error(cmd_->db);
    }

    bool query::query_iterator::operator==(query::query_iterator const& other) const
    {
        return rc_ == other.rc_;
    }

    bool query::query_iterator::operator!=(query::query_iterator const& other) const
    {
        return rc_ != other.rc_;
    }

    query::query_iterator& query::query_iterator::operator++()
    {
        rc_ = cmd_->step();
        if (rc_ != SQLITE_ROW && rc_ != SQLITE_DONE)
        throw db_error(cmd_->db);
        return *this;
    }

    query::query_iterator::value_type query::query_iterator::operator*() const
    {
        return rows(cmd_->stmt);
    }

    query::query(database& db, char const* stmt) : statement(db, stmt)
    {
    }
}