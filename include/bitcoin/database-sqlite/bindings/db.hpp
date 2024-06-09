//
// Created by dpr on 9/6/24.
//

#ifndef LIBBITCOIN_DATABASE_DATABASE_HPP
#define LIBBITCOIN_DATABASE_DATABASE_HPP

#include <sqlite3.h>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <string>
#include <tuple>

#include "noncopyable.hpp"

namespace db {
    class database : noncopyable {
        friend class statement;

        friend class db_error;

    public:
        // std::function is a general purpose polymorphic wrapper.
        // syntax -> std::function <return_type (argument_types)>
        using busy_handler = std::function<int(int)>;
        using commit_handler = std::function<int()>;
        using rollback_handler = std::function<void()>;
        using update_handler = std::function<void(int, char const *, char const *, long long int)>;
        using authorize_handler = std::function<int(int, char const *, char const *, char const *, char const *)>;
        using backup_handler = std::function<void(int, int, int)>;

        explicit database(char const *dbname = nullptr, int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                          const char *vfs = nullptr);

        database(database &&db);

        database &operator=(database &&db);

        ~database();

        int connect(char const *dbname, int flags, const char *vfs = nullptr);

        int disconnect();

        int attach(char const *dbname, char const *name);

        int detach(char const *name);

        int backup(database &dest_db, backup_handler h = {});

        int backup(char const* dbname, database& destdb, char const* destdbname, backup_handler h, int step_page = 5);

        // this function does not modify any of the object's member variable. (i.e. const keyword at last)
        // retrieves the last row id of the db
        //This line of code is a method declaration in C++. The method is named `last_insert_rowid` and it doesn't take any parameters. The `const` keyword at the end of the method declaration indicates that this method does not modify any of the object's member variables.
        //The return type of this method is `long long int`. This is a type of integer that can hold very large values. In the context of SQLite, `last_insert_rowid` is typically used to retrieve the ROWID (a unique identifier) of the last row inserted into a database. This can be useful in situations where you need to know the ID of a newly inserted row, for example, to use it as a foreign key in another table.
        //This can be useful in situations where you need to know the ID of a newly inserted row, for example, to use it as a foreign key in another table.

        long long int last_insert_row_id() const;

        // methods for changing configuration modes of db
        int enable_foreign_keys(bool enable = true);

        int enable_triggers(bool enable = true);

        int enable_extended_result_codes(bool enable = true);

        int changes() const;

        // error log methods
        char const *error_msg() const;

        int error_code() const;

        int extended_error_code() const;

        // for direct execution of statments we use exec, i assume we'll rarely do that

        int exec(char const *sql);

        int execf(char const *sql, ...);

        int set_busy_timeout(int ms);

        int set_busy_timeout(busy_handler h);

        void set_busy_handler(busy_handler h);

        void set_commit_handler(commit_handler h);

        void set_rollback_handler(rollback_handler h);

        void set_update_handler(update_handler h);

        void set_authorize_handler(authorize_handler h);

    private:
        database(sqlite3 *db);

        sqlite3 *db_;
        bool borrowing_;

        // handlers
        busy_handler bh_;
        commit_handler ch_;
        rollback_handler rh_;
        update_handler uh_;
        authorize_handler ah_;

    };
}

#endif //LIBBITCOIN_DATABASE_DATABASE_HPP
