#ifndef LIBBITCOIN_DATABASE_QUERY_HPP
#define LIBBITCOIN_DATABASE_QUERY_HPP

#include <sqlite3.h>
#include "db.hpp"
#include "statement.hpp"

namespace libbitcoin {
namespace database {
namespace sqlite {
namespace db {

    template <class T>
    struct convert {
        using to_int = int;
    };

    class query : public statement
    {
    public:
        class rows
        {
        public:

            class getstream
            {
            public:
                getstream(rows* rws, int idx);

                template <class T>
                getstream& operator >> (T& value) {
                value = rws_->get(idx_, T());
                ++idx_;
                return *this;
                }

            private:
                rows* rws_;
                int idx_;
            };

        explicit rows(sqlite3_stmt* stmt);

        int data_count() const;
        int column_type(int idx) const;

        int column_bytes(int idx) const;

        template <class T> T get(int idx) const {
            return get(idx, T());
        }

        template <class... Ts>
        std::tuple<Ts...> get_columns(typename convert<Ts>::to_int... idxs) const {
            return std::make_tuple(get(idxs, Ts())...);
        }

        getstream getter(int idx = 0);
        private:
        int get(int idx, int) const;
        double get(int idx, double) const;
        long long int get(int idx, long long int) const;
        char const* get(int idx, char const*) const;
        std::string get(int idx, std::string) const;
        void const* get(int idx, void const*) const;
        null_type get(int idx, null_type) const;

        private:
        sqlite3_stmt* stmt_;
        };

        class query_iterator
        : public std::iterator<std::input_iterator_tag, rows>
        {
        public:
        query_iterator();
        explicit query_iterator(query* cmd);

        bool operator==(query_iterator const&) const;
        bool operator!=(query_iterator const&) const;

        query_iterator& operator++();

        value_type operator*() const;

        private:
        query* cmd_;
        int rc_;
        };

        explicit query(database& db, char const* stmt = nullptr);

        int column_count() const;

        char const* column_name(int idx) const;
        char const* column_decltype(int idx) const;

        using iterator = query_iterator;

        iterator begin();
        iterator end();
    };

}
}
}
}

#endif