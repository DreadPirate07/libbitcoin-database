//
// Created by dpr on 9/6/24.
//

#ifndef LIBBITCOIN_DATABASE_STATEMENT_HPP
#define LIBBITCOIN_DATABASE_STATEMENT_HPP

#include <sqlite3.h>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <string>
#include <tuple>
#include "noncopyable.hpp"
#include "db.hpp"

namespace db {

    enum copy_semantic { copy, nocopy };
    class null_type {};
    extern null_type ignore;

    class statement : noncopyable {

    public:
        int prepare(char const *stmt);

        int finalize();

        // binding methods
        int bind(int idx, int value);

        int bind(int idx, double value);

        int bind(int idx, long long int value);

        int bind(int idx, char const *value, copy_semantic fcopy);

        int bind(int idx, void const *value, int n, copy_semantic fcopy);

        int bind(int idx, std::string const &value, copy_semantic fcopy);

        int bind(int idx);

        int bind(int idx, null_type);

        int bind(char const *name, int value);

        int bind(char const *name, double value);

        int bind(char const *name, long long int value);

        int bind(char const *name, char const *value, copy_semantic fcopy);

        int bind(char const *name, void const *value, int n, copy_semantic fcopy);

        int bind(char const *name, std::string const &value, copy_semantic fcopy);

        int bind(char const *name);

        int bind(char const *name, null_type);

        int step();

        int reset();

    protected:
        // db and statement objects.
        database &db;
        sqlite3_stmt *stmt;
        const char *tail;

        //prepares statement
        explicit statement(database& db, char const *stmt = nullptr);

        int prepare_impl(const char *stmt);

        int finalize_impl(sqlite3_stmt *stmt);

        ~statement();
    };
}
#endif //LIBBITCOIN_DATABASE_STATEMENT_HPP
