#ifndef LIBBITCOIN_DATABASE_COMMAND_HPP
#define LIBBITCOIN_DATABASE_COMMAND_HPP

#include <sqlite3.h>
#include "statement.hpp"
#include "db_error.hpp"
#include "db.hpp"


/**
 * The code is a nested class `bindstream` within 
 * the `command` class in C++. The `command` class is a subclass 
 * of `statement`. This `bindstream` class is designed to bind 
 * parameters to a SQL command.
 * 
 * The `bindstream` class has a constructor that takes a reference 
 * to a `command` object and an integer `idx` as parameters. The 
 * `command` object represents the SQL command to which parameters 
 * are to be bound, and `idx` represents the index of the parameter 
 * to be bound.
 * 
 * The `bindstream` class overloads the `<<` operator for different 
 * types of parameters. This operator is used to bind a value to the 
 * SQL command at the current index and then increment the index.
 * 
 * The `<<` operator is overloaded for three types of parameters:
 * 1. A template parameter `T`. This allows binding of various types 
 *    of parameters. The `bind` method of the `command` object is 
 *    called to bind the value to the command at the current index. 
 *    If the `bind` method returns a value other than `SQLITE_OK`, a 
 *    `database_error` is thrown. After the value is bound, the index 
 *    is incremented.
 * 2. A `char` pointer. This is used to bind a string literal to the 
 *    command. The `bind` method of the `command` object is called 
 *    with the `copy` policy, which means the string is copied into 
 *    the SQL command. If the `bind` method returns a value other than 
 *    `SQLITE_OK`, a `database_error` is thrown. After the value is 
 *    bound, the index is incremented.
 * 3. A `std::string` object. This is used to bind a `std::string` to 
 *    the command. The `bind` method of the `command` object is called 
 *    with the `copy` policy, which means the string is copied into 
 *    the SQL command. If the `bind` method returns a value other than 
 *    `SQLITE_OK`, a `database_error` is thrown. After the value is 
 *    bound, the index is incremented.
 * 
 * The `bindstream` class has two private members: a reference to a 
 * `command` object and an integer representing the current index of 
 * the parameter to be bound. These members are initialized by the 
 * constructor and updated by the `<<` operator.
 **/

namespace libbitcoin {
namespace sqlite {
namespace database {
namespace db{

    class command : public statement
    {
    public:
        class bindstream
        {
        public:
        bindstream(command& cmd, int idx);

        template <class T>
        bindstream& operator << (T value) {
            auto rc = cmd_.bind(idx_, value);
            if (rc != SQLITE_OK) {
            throw db_error(cmd_.db);
            }
            ++idx_;
            return *this;
        }
        bindstream& operator << (char const* value) {
            auto rc = cmd_.bind(idx_, value, copy);
            if (rc != SQLITE_OK) {
            throw db_error(cmd_.db);
            }
            ++idx_;
            return *this;
        }
        bindstream& operator << (std::string const& value) {
            auto rc = cmd_.bind(idx_, value, copy);
            if (rc != SQLITE_OK) {
            throw db_error(cmd_.db);
            }
            ++idx_;
            return *this;
        }

        private:
        command& cmd_;
        int idx_;
        };

    explicit command(database& db, char const* stmt = nullptr);

    bindstream binder(int idx = 1);

    int execute();
    int execute_all();
  };
}
}
}
}



#endif